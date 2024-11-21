#ifndef COMPONENTMAP_HPP
#define COMPONENTMAP_HPP

#include <cassert>
#include <unordered_map>
#include <vector>

#include "Entity.hpp"

namespace KumaECS
{
  /**
   * The interface for a ComponentMap.
   */
  class IComponentMap
  {
  public:
    virtual ~IComponentMap() = default;

    virtual bool ContainsComponent(Entity aEntity) const = 0;
    virtual void RemoveComponent(Entity aEntity) = 0;
  };

  /**
   * A ComponentMap maintains a packed vector of component data and maps Entity
   * IDs to indices in that vector.
   */
  template <typename T>
  class ComponentMap : public IComponentMap
  {
  public:
    ComponentMap<T>(size_t aMaxComponents)
    {
      mComponents.reserve(aMaxComponents);
    }

    bool ContainsComponent(Entity aEntity) const override
    {
      return mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end();
    }

    T &AddComponent(Entity aEntity)
    {
      assert(mEntityToIndexMap.find(aEntity) == mEntityToIndexMap.end());
      mEntityToIndexMap.emplace(aEntity, mComponents.size());
      mComponents.emplace_back();
      return mComponents.back();
    }

    void RemoveComponent(Entity aEntity) override
    {
      assert(mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end());
      auto index = mEntityToIndexMap.at(aEntity);
      mEntityToIndexMap.erase(aEntity);
      mComponents.erase(mComponents.begin() + index);

      for (auto &&entityPair : mEntityToIndexMap)
      {
        if (entityPair.second > index)
        {
          --entityPair.second;
        }
      }
    }

    T &GetComponent(Entity aEntity)
    {
      assert(mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end());
      return mComponents.at(mEntityToIndexMap.at(aEntity));
    }

  private:
    std::unordered_map<Entity, size_t> mEntityToIndexMap;
    std::vector<T> mComponents;
  };
} // namespace KumaECS

#endif // !COMPONENTMAP_HPP
