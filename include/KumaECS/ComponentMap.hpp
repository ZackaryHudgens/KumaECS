#ifndef COMPONENTMAP_HPP
#define COMPONENTMAP_HPP

#include <cassert>
#include <unordered_map>
#include <vector>

#include "Types.hpp"

namespace KumaECS {
// The interface for a ComponentMap.
class IComponentMap {
public:
  IComponentMap() = default;
  IComponentMap(IComponentMap &&) = default;
  IComponentMap(const IComponentMap &) = default;
  IComponentMap &operator=(IComponentMap &&) = default;
  IComponentMap &operator=(const IComponentMap &) = default;
  virtual ~IComponentMap() = default;

  virtual bool ContainsComponent(Entity aEntity) const = 0;
  virtual void RemoveComponent(Entity aEntity) = 0;
};

// A ComponentMap maintains a packed vector of component data and maps Entity
// IDs to indices in that vector.
template <typename T> class ComponentMap : public IComponentMap {
public:
  ComponentMap<T>(size_t aMaxComponents) : mSize(0) {
    mComponents.reserve(aMaxComponents);
    for (size_t i = 0; i < aMaxComponents; ++i) {
      mComponents.emplace_back();
    }
  }

  ComponentMap<T>(ComponentMap<T> &&) = default;
  ComponentMap<T>(const ComponentMap<T> &) = default;
  ComponentMap<T> &operator=(ComponentMap<T> &&) = default;
  ComponentMap<T> &operator=(const ComponentMap<T> &) = default;
  ~ComponentMap<T>() = default;

  bool ContainsComponent(Entity aEntity) const override {
    return mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end();
  }

  T &AddComponent(Entity aEntity) {
    assert(mSize < mComponents.size());
    assert(mEntityToIndexMap.find(aEntity) == mEntityToIndexMap.end());

    mEntityToIndexMap[aEntity] = mSize;
    mIndexToEntityMap[mSize] = aEntity;
    mComponents[mEntityToIndexMap[aEntity]] = T();
    ++mSize;

    return mComponents[mEntityToIndexMap[aEntity]];
  }

  void RemoveComponent(Entity aEntity) override {
    assert(mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end());

    auto lastValidIndex = mSize - 1;
    auto removedIndex = mEntityToIndexMap[aEntity];
    auto movedEntity = mIndexToEntityMap[lastValidIndex];

    mComponents[removedIndex] = std::move(mComponents[lastValidIndex]);
    mEntityToIndexMap[movedEntity] = mEntityToIndexMap[aEntity];
    mIndexToEntityMap[removedIndex] = movedEntity;

    mEntityToIndexMap.erase(aEntity);
    mIndexToEntityMap.erase(lastValidIndex);

    --mSize;
  }

  T &GetComponent(Entity aEntity) {
    assert(mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end());
    return mComponents[mEntityToIndexMap[aEntity]];
  }

private:
  std::unordered_map<Entity, size_t> mEntityToIndexMap;
  std::unordered_map<size_t, Entity> mIndexToEntityMap;
  std::vector<T> mComponents;

  size_t mSize;
};
} // namespace KumaECS

#endif // !COMPONENTMAP_HPP
