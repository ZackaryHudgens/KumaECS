#ifndef ECS_HPP
#define ECS_HPP

#include <bitset>
#include <cassert>
#include <cstddef>
#include <memory>
#include <queue>
#include <set>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace KumaECS {

/*******************************************************************************
 * Definitions
 ******************************************************************************/
const size_t MAX_COMPONENT_TYPES = 64;
const size_t MAX_ENTITIES = 5000;

using Entity = size_t;
using EntitySet = std::set<Entity>;
using Signature = std::bitset<MAX_COMPONENT_TYPES>;

/*******************************************************************************
 * System
 ******************************************************************************/
class System
{
  public:
    EntitySet mEntities;
};

/*******************************************************************************
 * ComponentMap
 ******************************************************************************/
class IComponentMap
{
  public:
    virtual bool ContainsComponent(Entity aEntity) const = 0;
    virtual void RemoveComponent(Entity aEntity) = 0;
};

template <typename T>
class ComponentMap : public IComponentMap
{
  public:
    ComponentMap<T>(size_t aMaxComponents)
      : mSize(0)
    {
      mComponents.reserve(aMaxComponents);
      for(size_t i = 0; i < aMaxComponents; ++i)
      {
        mComponents.emplace_back();
      }
    }

    bool ContainsComponent(Entity aEntity) const override
    {
      return mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end();
    }

    T& AddComponent(Entity aEntity)
    {
      assert(mSize < mComponents.size());
      assert(mEntityToIndexMap.find(aEntity) == mEntityToIndexMap.end());

      mEntityToIndexMap[aEntity] = mSize;
      mIndexToEntityMap[mSize] = aEntity;
      mComponents[mEntityToIndexMap[aEntity]] = T();
      ++mSize;

      return mComponents[mEntityToIndexMap[aEntity]];
    }

    void RemoveComponent(Entity aEntity) override
    {
      assert(mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end());

      auto lastValidIndex = mSize - 1;
      auto removedIndex = mEntityToIndexMap[aEntity];
      auto movedEntity = mIndexToEntityMap[lastValidIndex];

      mComponents[removedIndex] = mComponents[lastValidIndex];
      mEntityToIndexMap[movedEntity] = mEntityToIndexMap[aEntity];
      mIndexToEntityMap[removedIndex] = movedEntity;

      mEntityToIndexMap.erase(aEntity);
      mIndexToEntityMap.erase(lastValidIndex);

      --mSize;
    }

    T& GetComponent(Entity aEntity)
    {
      assert(mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end());
      return mComponents[mEntityToIndexMap[aEntity]];
    }

  private:
    std::unordered_map<Entity, size_t> mEntityToIndexMap;
    std::unordered_map<size_t, Entity> mIndexToEntityMap;
    std::vector<T> mComponents;

    size_t mSize;
};

/*******************************************************************************
 * Scene
 ******************************************************************************/
class Scene
{
  public:
    Scene()
    {
      for(size_t i = 0; i < MAX_ENTITIES; ++i)
      {
        mAvailableEntities.push(i);
        mEntitySignatures.emplace_back();
      }
    }

    Entity CreateEntity()
    {
      assert(!mAvailableEntities.empty());

      Entity entity = mAvailableEntities.front();
      mAvailableEntities.pop();
      return entity;
    }

    void DestroyEntity(Entity aEntity)
    {
      mAvailableEntities.push(aEntity);
      mEntitySignatures[aEntity].reset();

      for(auto& system : mSystems)
      {
        system->mEntities.erase(aEntity);
      }

      for(auto& map : mComponentMaps)
      {
        if(map->ContainsComponent(aEntity))
        {
          map->RemoveComponent(aEntity);
        }
      }
    }

    template<typename T>
    void RegisterComponentType(size_t aMax)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) == mComponentToIndexMap.end());

      mComponentToIndexMap[name] = mComponentMaps.size();
      mComponentMaps.emplace_back(std::make_unique<ComponentMap<T>>(aMax));
    }

    template<typename T>
    void AddComponentToSignature(Signature& aSignature)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

      aSignature.set(mComponentToIndexMap[name]);
    }

    template<typename T>
    T& AddComponentToEntity(Entity aEntity)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

      mEntitySignatures[aEntity].set(mComponentToIndexMap[name]);

      for(size_t i = 0; i < mSystems.size(); ++i)
      {
        auto systemSignature = mSystemSignatures[i];
        if((systemSignature & mEntitySignatures[aEntity]) == systemSignature)
        {
          mSystems[i]->mEntities.insert(aEntity);
        }
      }

      auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
      return static_cast<ComponentMap<T>*>(componentMap)->AddComponent(aEntity);
    }

    template<typename T>
    void RemoveComponentFromEntity(Entity aEntity)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

      auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
      static_cast<ComponentMap<T>*>(componentMap)->RemoveComponent(aEntity);
      mEntitySignatures[aEntity].reset(mComponentToIndexMap[name]);

      for(size_t i = 0; i < mSystems.size(); ++i)
      {
        auto systemSignature = mSystemSignatures[i];
        if((systemSignature & mEntitySignatures[aEntity]) != systemSignature)
        {
          mSystems[i]->mEntities.erase(aEntity);
        }
      }
    }

    template<typename T>
    T& GetComponentForEntity(Entity aEntity)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

      auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
      return static_cast<ComponentMap<T>*>(componentMap)->GetComponent(aEntity);
    }

    template<typename T>
    T* RegisterSystemType(const Signature& aSignature)
    {
      auto name = typeid(T).name();
      assert(mSystemToIndexMap.find(name) == mSystemToIndexMap.end());

      mSystemToIndexMap.emplace(name, mSystemSignatures.size());
      mSystemSignatures.emplace_back(aSignature);
      mSystems.emplace_back(std::make_unique<T>());
      return static_cast<T*>(mSystems.back().get());
    }

  private:
    std::queue<Entity> mAvailableEntities;
    std::vector<Signature> mEntitySignatures;

    std::unordered_map<const char*, size_t> mSystemToIndexMap;
    std::vector<Signature> mSystemSignatures;
    std::vector<std::unique_ptr<System>> mSystems;

    std::unordered_map<const char*, size_t> mComponentToIndexMap;
    std::vector<std::unique_ptr<IComponentMap>> mComponentMaps;
};

} // namespace KumaECS

#endif
