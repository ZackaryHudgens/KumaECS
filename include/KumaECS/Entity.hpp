#ifndef ENTITY_HPP
#define ENTITY_HPP

namespace KumaECS {
class Entity {
 public:
  Entity(unsigned int aID) : mID(aID) {}

  unsigned int GetID() const { return mID; }

 private:
  unsigned int mID;
};
}  // namespace KumaECS

#endif