#ifndef ENTITY_HPP
#define ENTITY_HPP

namespace KumaECS {
/**
 * An Entity is just an ID. These are used internally by the Scene and various
 * other classes to keep track of which components are tied together. Externally
 * (e.g. in a System), EntityHandles should be used.
 */
class Entity {
 public:
  Entity(unsigned int aID) : mID(aID) {}

  unsigned int GetID() const { return mID; }
  bool IsValid() const { return mID != 0; }

  friend bool operator<(const Entity& lhs, Entity& rhs) {
    return lhs.mID < rhs.mID;
  }

 private:
  unsigned int mID;
};
}  // namespace KumaECS

#endif