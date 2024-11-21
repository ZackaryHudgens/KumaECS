#include "KumaECS/Scene.hpp"

#include "KumaECS/EntityHandle.hpp"
#include "KumaECS/SignatureHandle.hpp"

namespace KumaECS
{
  /*****************************************************************************/
  EntityHandle Scene::CreateEntity()
  {
    return EntityHandle(mEntityFactory.CreateEntity(), *this);
  }

  /*****************************************************************************/
  void Scene::DestroyEntity(Entity aEntity)
  {
    mEntityFactory.DestroyEntity(aEntity);

    // Remove the Entity from all Systems.
    for (auto &system : mSystems)
    {
      system->mEntities.erase(EntityHandle(aEntity, *this));
    }

    // Remove the Entity's old component data.
    for (auto &map : mComponentMaps)
    {
      if (map->ContainsComponent(aEntity))
      {
        map->RemoveComponent(aEntity);
      }
    }
  }

  /*****************************************************************************/
  void Scene::AddSystem(std::unique_ptr<System> aSystem)
  {
    mSystems.emplace_back(std::move(aSystem));

    SignatureHandle sig(*this);
    mSystems.back()->SetSignature(sig);

    mSystemSignatures.emplace_back(sig.GetSignatureRaw());
  }

  /*****************************************************************************/
  void Scene::OperateSystems(double dt)
  {
    for (auto &system : mSystems)
    {
      system->Operate(*this, dt);
    }
  }

  /*****************************************************************************/
  void Scene::AddEntityToRelevantSystems(Entity aEntity)
  {
    auto entitySignature = mEntitySignatures.at(aEntity.GetID());
    for (size_t i = 0; i < mSystems.size(); ++i)
    {
      auto systemSignature = mSystemSignatures[i];
      if (systemSignature.Matches(entitySignature))
      {
        mSystems.at(i)->mEntities.insert(EntityHandle(aEntity, *this));
      }
    }
  }

  /*****************************************************************************/
  void Scene::RemoveEntityFromRelevantSystems(Entity aEntity) {}
} // namespace KumaECS
