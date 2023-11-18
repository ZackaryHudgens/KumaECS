#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include "ComponentMap.hpp"
#include "Entity.hpp"
#include "EntityFactory.hpp"
#include "System.hpp"

namespace KumaECS {
class EntityHandle;

/**
 * A Scene contains all the information necessary for an area or level of a
 * game. This includes a list of Entities, a list of Systems, and a ComponentMap
 * for each type of component.
 *
 * A game may contain multiple Scenes; for example, there may be separate Scenes
 * for a main menu, a main gameplay loop, and a credits screen.
 */
class Scene {
 public:
  /****************************************************************************
   * Entity methods
   ***************************************************************************/

  EntityHandle CreateEntity();
  void DestroyEntity(EntityHandle aEntity);

  /****************************************************************************
   * Component methods
   ***************************************************************************/

  template <typename T>
  void RegisterComponentType(size_t aMax) {
    auto name = typeid(T).name();
    assert(mComponentToIndexMap.find(name) == mComponentToIndexMap.end());

    // Create a map for the new component type.
    mComponentToIndexMap[name] = mComponentMaps.size();
    mComponentMaps.emplace_back(std::make_unique<ComponentMap<T>>(aMax));

    // Update all existing Signatures.
    for (auto &&sig : mEntitySignatures) {
      sig.AddExtra();
    }

    for (auto &&sig : mSystemSignatures) {
      sig.AddExtra();
    }
  }

  template <typename T>
  void GetComponentIndex() {
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
  T &AddComponentToEntity(Entity aEntity) {
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

  template <typename T>
  void RemoveComponentFromEntity(Entity aEntity) {
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

  template <typename T>
  T &GetComponentForEntity(Entity aEntity) {
    auto name = typeid(T).name();
    assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

    auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
    return static_cast<ComponentMap<T> *>(componentMap)->GetComponent(aEntity);
  }

  template <typename T>
  bool DoesEntityHaveComponent(Entity aEntity) {
    auto name = typeid(T).name();
    assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

    auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
    return static_cast<ComponentMap<T> *>(componentMap)
        ->ContainsComponent(aEntity);
  }

  /*
    EntitySet GetEntitiesWithSignature(const Signature &aSignature) {
      EntitySet entities;
      for (size_t i = 0; i < mEntitySignatures.size(); ++i) {
        if ((mEntitySignatures[i] & aSignature) == aSignature) {
          entities.insert(i);
        }
      }

      return entities;
    }

    template <typename T>
    EntitySet GetEntitiesWithComponent() {
      Signature sig;
      AddComponentToSignature<T>(sig);
      return GetEntitiesWithSignature(sig);
    }
    */

 private:
  EntityFactory mEntityFactory;

  std::vector<Signature> mEntitySignatures;

  std::vector<Signature> mSystemSignatures;
  std::vector<std::unique_ptr<System>> mSystems;

  std::unordered_map<const char *, size_t> mComponentToIndexMap;
  std::vector<std::unique_ptr<IComponentMap>> mComponentMaps;
};
}  // namespace KumaECS

#endif  // !SCENE_HPP
