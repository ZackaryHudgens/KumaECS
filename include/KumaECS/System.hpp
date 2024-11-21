#ifndef SYSTEM_HPP
#define SYSTEM_HPP

// #include "EntityHandle.hpp"
// #include "SignatureHandle.hpp"

namespace KumaECS
{
  class Scene;
  class SignatureHandle;
  class EntitySet;

  /**
   * A System contains a set of EntityHandles that share a Signature. This set is
   * managed by a Scene as Entities are created and deleted, and as components are
   * added and removed.
   */
  class System
  {
    friend class Scene;

  public:
    virtual ~System() = default;

    virtual void SetSignature(SignatureHandle &aSignature) = 0;
    virtual void Operate(Scene &aScene, double dt) = 0;

    const EntitySet &GetEntities() const { return *mEntities; }

  private:
    EntitySet *mEntities;
  };
} // namespace KumaECS

#endif // !SYSTEM_HPP
