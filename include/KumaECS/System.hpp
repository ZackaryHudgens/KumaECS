#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Types.hpp"

namespace KumaECS
{
  class Scene;

  // A System contains a set of Entities that share a Signature. This set is
  // managed by a Scene as Entities are created and deleted, and as components are
  // added and removed.
  class System
  {
    friend class Scene;

  public:
    System() = default;
    System(System &&) = default;
    System(const System &) = default;
    System &operator=(System &&) = default;
    System &operator=(const System &) = default;
    virtual ~System() = default;

    virtual void Operate(Scene &aScene, double dt) {}

    const EntitySet &GetEntities() const { return mEntities; }

  private:
    EntitySet mEntities;
  };
} // namespace KumaECS

#endif // !SYSTEM_HPP
