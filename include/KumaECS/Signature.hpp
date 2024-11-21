#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

#include <bitset>

namespace KumaECS
{
  // The maximum number of component types that a single Scene can keep track of.
  const size_t MAX_COMPONENT_TYPES = 128;

  // A set of flags used by the Scene class to keep track of which components an
  // Entity is associated with, as well as which Entities each System is
  // interested in.
  using Signature = std::bitset<MAX_COMPONENT_TYPES>;
} // namespace KumaECS

#endif // !SIGNATURE_HPP