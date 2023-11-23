#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

#include <cstddef>
#include <vector>

namespace KumaECS {
class Scene;

/**
 * A Signature is a means of keeping track of which component types
 * an Entity has, as well as which component types a System in interested in.
 */
class Signature {
  friend class Scene;

 public:
  Signature(size_t aSize) : mIndices(aSize, 0) {}

  void Set(size_t aIndex) { mIndices.at(aIndex) = 1; }
  void Unset(size_t aIndex) { mIndices.at(aIndex) = 0; }

  bool Matches(const Signature& aSignature) const {
    if (aSignature.mIndices.size() != mIndices.size()) {
      return false;
    }

    bool matchFound = true;
    for (size_t i = 0; i < aSignature.mIndices.size(); ++i) {
      if (mIndices.at(i) == 1 && aSignature.mIndices.at(i) == 0) {
        matchFound = false;
        break;
      }
    }

    return matchFound;
  }

 private:
  std::vector<bool> mIndices;
};
}  // namespace KumaECS

#endif