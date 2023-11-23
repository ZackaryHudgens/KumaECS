#ifndef ENTITYHANDLE_HPP
#define ENTITYHANDLE_HPP

#include <set>

#include "Entity.hpp"
#include "Scene.hpp"

namespace KumaECS {
/**
 * An EntityHandle is a reference to an internal Entity in a Scene. These are
 * primarily used for external code, e.g. for user-defined Systems.
 */
class EntityHandle {
 public:
  EntityHandle(Entity aEntity, Scene& aScene)
      : mEntity(aEntity), mScene(&aScene) {}

  Entity GetEntityRaw() const { return mEntity; }
  bool IsValid() const { return mEntity.IsValid(); }

  void Destroy() { mScene->DestroyEntity(mEntity); }

  template <typename T>
  void AddComponent() {
    mScene->AddComponentToEntity<T>(mEntity);
  }

  template <typename T>
  void RemoveComponent() {
    mScene->RemoveComponentFromEntity<T>(mEntity);
  }

  template <typename T>
  T& GetComponent() {
    return mScene->GetComponentForEntity<T>(mEntity);
  }

  friend bool operator<(const EntityHandle& lhs, EntityHandle& rhs) {
    return lhs.mEntity < rhs.mEntity;
  }

 private:
  Entity mEntity;
  Scene* mScene;
};

using EntitySet = std::set<EntityHandle>;
}  // namespace KumaECS

#endif