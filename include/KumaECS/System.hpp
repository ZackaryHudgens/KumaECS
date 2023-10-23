#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Types.hpp"

namespace KumaECS {
// A System contains a set of Entities that share a Signature. This set is
// managed by a Scene as Entities are created and deleted, and as components are
// added and removed.
class System {
public:
  virtual ~System() = default;

  EntitySet mEntities;
};
} // namespace KumaECS

#endif // !SYSTEM_HPP
