#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include <cstddef>
#include <queue>

namespace KumaECS
{
  /**
   * The EntityFactory is used to create unique IDs for Entities in a Scene. The
   * first unique ID created via CreateEntity() is 1; this means that any Entity
   * with an ID of 0 can be considered invalid.
   */
  class EntityFactory
  {
  public:
    EntityFactory() : mCount(1) {}

    Entity CreateEntity()
    {
      size_t id = 0;
      if (!mAvailableIDs.empty())
      {
        id = mAvailableIDs.front();
        mAvailableIDs.pop();
      }
      else
      {
        id = mCount;
        ++mCount;
      }

      return Entity(id);
    }

    void DestroyEntity(Entity aEntity) { mAvailableIDs.push(aEntity.GetID()); }

  private:
    size_t mCount;
    std::queue<size_t> mAvailableIDs;
  };
} // namespace KumaECS

#endif