#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include "ComponentMap.hpp"
#include "Entity.hpp"
#include "EntityFactory.hpp"
#include "Signature.hpp"
#include "System.hpp"

namespace KumaECS
{
  class EntityHandle;

  /**
   * A Scene contains all the information necessary for an area or level of a
   * game. This includes a list of Entities, a list of Systems, and a ComponentMap
   * for each type of component.
   *
   * A game may contain multiple Scenes; for example, there may be separate Scenes
   * for a main menu, a main gameplay loop, and a credits screen.
   */
  class Scene
  {
  public:
    /****************************************************************************
     * Entity methods
     ***************************************************************************/

    EntityHandle CreateEntity();
    void DestroyEntity(Entity aEntity);

    /****************************************************************************
     * Component methods
     ***************************************************************************/

    template <typename T>
    void RegisterComponentType(size_t aMax)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) == mComponentToIndexMap.end());

      // Create a map for the new component type.
      mComponentToIndexMap.emplace(name, mComponentMaps.size());
      mComponentMaps.emplace_back(std::make_unique<ComponentMap<T>>(aMax));

      // Update each Entity's Signature.
      for (auto &&sig : mEntitySignatures)
      {
        sig.mIndices.emplace_back(0);
      }

      // Update each System's Signature.
      for (auto &&sig : mSystemSignatures)
      {
        sig.mIndices.emplace_back(0);
      }
    }

    template <typename T>
    size_t GetComponentIndex()
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());
      return mComponentToIndexMap.at(name);
    }

    size_t GetNumComponentTypes() const { return mComponentMaps.size(); }

    /****************************************************************************
     * System methods
     ***************************************************************************/

    void AddSystem(std::unique_ptr<System> aSystem);
    void OperateSystems(double dt);

    /****************************************************************************
     * Entity-Component interoperational methods
     ***************************************************************************/

    template <typename T>
    T &AddComponentToEntity(Entity aEntity)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

      // Update the Entity's Signature and add it to any relevant Systems.
      auto &entitySignature = mEntitySignatures.at(aEntity.GetID());
      entitySignature.Set(mComponentToIndexMap.at(name));
      AddEntityToRelevantSystems(aEntity);

      // Create the component and return a reference.
      auto componentMap = mComponentMaps.at(mComponentToIndexMap.at(name)).get();
      return static_cast<ComponentMap<T> *>(componentMap)->AddComponent(aEntity);
    }

    template <typename T>
    void RemoveComponentFromEntity(Entity aEntity)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

      // Update the Entity's Signature and remove it from any relevant Systems.
      auto &entitySignature = mEntitySignatures.at(aEntity.GetID());
      entitySignature.Set(mComponentToIndexMap.at(name));
      RemoveEntityFromRelevantSystems(aEntity);

      // Remove the component from the component map.
      auto componentMap = mComponentMaps.at(mComponentToIndexMap.at(name)).get();
      static_cast<ComponentMap<T> *>(componentMap)->RemoveComponent(aEntity);
    }

    template <typename T>
    T &GetComponentForEntity(Entity aEntity)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

      auto componentMap = mComponentMaps.at(mComponentToIndexMap.at(name)).get();
      return static_cast<ComponentMap<T> *>(componentMap)->GetComponent(aEntity);
    }

  private:
    void AddEntityToRelevantSystems(Entity aEntity);
    void RemoveEntityFromRelevantSystems(Entity aEntity);

    EntityFactory mEntityFactory;

    std::vector<Signature> mEntitySignatures;
    std::vector<Signature> mSystemSignatures;

    std::vector<std::unique_ptr<System>> mSystems;

    std::unordered_map<const char *, size_t> mComponentToIndexMap;
    std::vector<std::unique_ptr<IComponentMap>> mComponentMaps;
  };
} // namespace KumaECS

#endif // !SCENE_HPP
