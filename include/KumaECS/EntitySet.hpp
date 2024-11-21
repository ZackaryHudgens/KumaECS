#ifndef ENTITYSET_HPP
#define ENTITYSET_HPP

#include <set>

#include "Entity.hpp"

namespace KumaECS
{
  // A set of unique Entity IDs.
  using EntitySet = std::set<Entity>;
} // namespace KumaECS

#endif // !ENTITYSET_HPP