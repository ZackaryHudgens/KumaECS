#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <queue>
#include <vector>

#include "ComponentMap.hpp"
#include "System.hpp"
#include "Types.hpp"

namespace KumaECS {
// A Scene contains all the data necessary for a single screen/area of a game.
// This includes a list of Entity IDs, a list of Systems, and a ComponentMap for
// each type of component.
class Scene {
public:
  Scene(size_t aMaxEntities) {
    for (size_t i = 0; i < aMaxEntities; ++i) {
      mAvailableEntities.push(i);
      mEntitySignatures.emplace_back();
    }
  }

  Scene(Scene &&) = default;
  Scene(const Scene &) = default;
  Scene &operator=(Scene &&) = default;
  Scene &operator=(const Scene &) = default;
  ~Scene() = default;

  Entity CreateEntity() {
    assert(!mAvailableEntities.empty());

    Entity entity = mAvailableEntities.front();
    mAvailableEntities.pop();
    return entity;
  }

  void DestroyEntity(Entity aEntity) {
    mAvailableEntities.push(aEntity);
    mEntitySignatures[aEntity].reset();

    for (auto &system : mSystems) {
      system->mEntities.erase(aEntity);
    }

    for (auto &map : mComponentMaps) {
      if (map->ContainsComponent(aEntity)) {
        map->RemoveComponent(aEntity);
      }
    }
  }

  template <typename T> void RegisterComponentType(size_t aMax) {
    auto name = typeid(T).name();
    assert(mComponentToIndexMap.find(name) == mComponentToIndexMap.end());

    mComponentToIndexMap[name] = mComponentMaps.size();
    mComponentMaps.emplace_back(std::make_unique<ComponentMap<T>>(aMax));
  }

  template <typename T> void AddComponentToSignature(Signature &aSignature) {
    auto name = typeid(T).name();
    assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

    aSignature.set(mComponentToIndexMap[name]);
  }

  template <typename T> T &AddComponentToEntity(Entity aEntity) {
    auto name = typeid(T).name();
    assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

    mEntitySignatures[aEntity].set(mComponentToIndexMap[name]);

    for (size_t i = 0; i < mSystems.size(); ++i) {
      auto systemSignature = mSystemSignatures[i];
      if ((systemSignature & mEntitySignatures[aEntity]) == systemSignature) {
        mSystems[i]->mEntities.insert(aEntity);
      }
    }

    auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
    return static_cast<ComponentMap<T> *>(componentMap)->AddComponent(aEntity);
  }

  template <typename T> void RemoveComponentFromEntity(Entity aEntity) {
    auto name = typeid(T).name();
    assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

    auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
    static_cast<ComponentMap<T> *>(componentMap)->RemoveComponent(aEntity);
    mEntitySignatures[aEntity].reset(mComponentToIndexMap[name]);

    for (size_t i = 0; i < mSystems.size(); ++i) {
      auto systemSignature = mSystemSignatures[i];
      if ((systemSignature & mEntitySignatures[aEntity]) != systemSignature) {
        mSystems[i]->mEntities.erase(aEntity);
      }
    }
  }

  template <typename T> T &GetComponentForEntity(Entity aEntity) {
    auto name = typeid(T).name();
    assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

    auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
    return static_cast<ComponentMap<T> *>(componentMap)->GetComponent(aEntity);
  }

  template <typename T> bool DoesEntityHaveComponent(Entity aEntity) {
    auto name = typeid(T).name();
    assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

    auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
    return static_cast<ComponentMap<T> *>(componentMap)
        ->ContainsComponent(aEntity);
  }

  std::vector<Entity> GetEntitiesWithSignature(const Signature &aSignature) {
    std::vector<Entity> entities;
    for (size_t i = 0; i < mEntitySignatures.size(); ++i) {
      if ((mEntitySignatures[i] & aSignature) == aSignature) {
        entities.emplace_back(i);
      }
    }

    return entities;
  }

  template <typename T> T *RegisterSystemType(const Signature &aSignature) {
    auto name = typeid(T).name();
    assert(mSystemToIndexMap.find(name) == mSystemToIndexMap.end());

    mSystemToIndexMap.emplace(name, mSystemSignatures.size());
    mSystemSignatures.emplace_back(aSignature);
    mSystems.emplace_back(std::make_unique<T>());
    return static_cast<T *>(mSystems.back().get());
  }

private:
  std::queue<Entity> mAvailableEntities;
  std::vector<Signature> mEntitySignatures;

  std::unordered_map<const char *, size_t> mSystemToIndexMap;
  std::vector<Signature> mSystemSignatures;
  std::vector<std::unique_ptr<System>> mSystems;

  std::unordered_map<const char *, size_t> mComponentToIndexMap;
  std::vector<std::unique_ptr<IComponentMap>> mComponentMaps;
};
} // namespace KumaECS

#endif // !SCENE_HPP
