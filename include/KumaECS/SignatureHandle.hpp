#ifndef SIGNATUREHANDLE_HPP
#define SIGNATUREHANDLE_HPP

#include "Scene.hpp"

namespace KumaECS {
class SignatureHandle {
 public:
  SignatureHandle(Scene& aScene) : mScene(&aScene) {}

 private:
  Scene* mScene;
};
}  // namespace KumaECS

#endif