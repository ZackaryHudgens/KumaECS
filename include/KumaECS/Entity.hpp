#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstddef>

namespace KumaECS
{
  /**
   * An Entity is just an ID. These are used internally by the Scene and various
   * other classes to keep track of which components are tied together. Externally
   * (e.g. in a System), EntityHandles should be used.
   */
  class Entity
  {
  public:
    Entity(size_t aID) : mID(aID) {}

    size_t GetID() const { return mID; }
    bool IsValid() const { return mID != 0; }

    friend bool operator<(const Entity &lhs, Entity &rhs)
    {
      return lhs.mID < rhs.mID;
    }

  private:
    size_t mID;
  };
} // namespace KumaECS

#endif