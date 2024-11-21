#ifndef SIGNATUREHANDLE_HPP
#define SIGNATUREHANDLE_HPP

#include "Scene.hpp"
#include "Signature.hpp"

namespace KumaECS
{
  /**
   * A SignatureHandle provides an easy way for a System to specify which
   * component types it is interested in.
   */
  class SignatureHandle
  {
  public:
    SignatureHandle(Scene &aScene)
        : mScene(&aScene), mSignature(aScene.GetNumComponentTypes()) {}

    template <typename T>
    void AddComponent()
    {
      mSignature.Set(mScene->GetComponentIndex<T>());
    }

    template <typename T>
    void RemoveComponent()
    {
      mSignature.Unset(mScene->GetComponentIndex<T>());
    }

    Signature &GetSignatureRaw() { return mSignature; }

  private:
    Scene *mScene;
    Signature mSignature;
  };
} // namespace KumaECS

#endif