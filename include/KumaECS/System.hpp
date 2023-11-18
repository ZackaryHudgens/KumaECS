#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "EntityHandle.hpp"

namespace KumaECS {
class Scene;

/**
 * A System contains a set of EntityHandles that share a Signature. This set is
 * managed by a Scene as Entities are created and deleted, and as components are
 * added and removed.
 */
class System {
  friend class Scene;

 public:
  virtual ~System() = default;

  virtual void Operate(Scene &aScene, double dt) = 0;

  const EntityHandleSet &GetEntities() const { return mEntities; }

 private:
  EntityHandleSet mEntities;
};
}  // namespace KumaECS

#endif  // !SYSTEM_HPP
