#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstddef>

namespace KumaECS
{
  // In KumaECS, an Entity is just a unique ID. These Entities can have data
  // associated with them via the ComponentMap class.
  using Entity = size_t;
} // namespace KumaECS

#endif // !ENTITY_HPP