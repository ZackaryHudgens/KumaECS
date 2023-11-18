#ifndef TYPES_HPP
#define TYPES_HPP

#include <bitset>
#include <cstddef>
#include <set>

namespace KumaECS {
// The maximum number of component types that a single Scene can keep track of.
const size_t MAX_COMPONENT_TYPES = 64;

// A set of flags used by the Scene class to keep track of which components an
// Entity is associated with, as well as which Entities each System is
// interested in.
using Signature = std::bitset<MAX_COMPONENT_TYPES>;
}  // namespace KumaECS

#endif  // !TYPES_HPP
