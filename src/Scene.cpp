#include "KumaECS/Scene.hpp"

#include "KumaECS/EntityHandle.hpp"
#include "KumaECS/SignatureHandle.hpp"

namespace KumaECS {
/*****************************************************************************/
EntityHandle Scene::CreateEntity() {
  return EntityHandle(mEntityFactory.CreateEntity(), *this);
}

/*****************************************************************************/
void Scene::DestroyEntity(EntityHandle aEntity) {
  mEntityFactory.DestroyEntity(aEntity.GetEntityRaw());

  // Remove the Entity from all Systems.
  for (auto &system : mSystems) {
    system->mEntities.erase(aEntity);
  }

  // Remove the Entity's old component data.
  for (auto &map : mComponentMaps) {
    if (map->ContainsComponent(aEntity.GetEntityRaw())) {
      map->RemoveComponent(aEntity.GetEntityRaw());
    }
  }
}

/*****************************************************************************/
void Scene::AddSystem(std::unique_ptr<System> aSystem) {
  mSystems.emplace_back(std::move(aSystem));

  SignatureHandle sig(*this);
  mSystems.back()->SetSignature(sig);

  mSystemSignatures.emplace_back(sig.GetSignatureRaw());
}

/*****************************************************************************/
void Scene::OperateSystems(double dt) {
  for (auto &system : mSystems) {
    system->Operate(*this, dt);
  }
}
}  // namespace KumaECS
