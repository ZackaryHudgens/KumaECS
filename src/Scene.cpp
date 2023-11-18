#include "KumaECS/Scene.hpp"

#include "KumaECS/EntityHandle.hpp"

namespace KumaECS {
/*****************************************************************************/
void Scene::OperateSystems(double dt) {
  for (auto &system : mSystems) {
    system->Operate(*this, dt);
  }
}

/*****************************************************************************/
EntityHandle Scene::CreateEntity() {
  return EntityHandle(mEntityFactory.CreateEntity(), *this);
}

/*****************************************************************************/
void Scene::DestroyEntity(EntityHandle aEntity) {
  mEntityFactory.DestroyEntity(aEntity.GetEntity());
}
}  // namespace KumaECS
