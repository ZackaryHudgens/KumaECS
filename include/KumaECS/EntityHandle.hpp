#ifndef ENTITYHANDLE_HPP
#define ENTITYHANDLE_HPP

#include "Entity.hpp"
#include "Scene.hpp"

namespace KumaECS {
class EntityHandle {
 public:
  EntityHandle(Entity aEntity, Scene& aScene)
      : mEntity(aEntity), mScene(&aScene) {}
  ~EntityHandle() {}

  template <typename T>
  void AddComponent() {}

  template <typename T>
  void RemoveComponent() {}

  template <typename T>
  T& GetComponent() {
    return T();
  }

 private:
  Entity mEntity;
  Scene* mScene;
};
}  // namespace KumaECS

#endif