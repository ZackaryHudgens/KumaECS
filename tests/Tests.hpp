#include "KumaECS/ComponentMap.hpp"
#include "KumaECS/Scene.hpp"
#include "KumaECS/System.hpp"
#include "KumaECS/Types.hpp"

#include <cassert>
#include <string>

namespace KumaECS {

struct ComponentA {
  int value{0};
};

struct ComponentB {
  std::string value;
};

class SystemA : public System {};
class SystemB : public System {};
class SystemC : public System {};

/******************************************************************************/
inline void TestComponentMap() {
  ComponentMap<ComponentA> map(100);

  map.AddComponent(0);
  map.AddComponent(1);
  map.AddComponent(2);
  map.AddComponent(3);
  map.AddComponent(4);

  assert(map.ContainsComponent(0));
  assert(map.ContainsComponent(1));
  assert(map.ContainsComponent(2));
  assert(map.ContainsComponent(3));
  assert(map.ContainsComponent(4));
  assert(!map.ContainsComponent(5));

  map.GetComponent(0).value = 5;
  map.GetComponent(1).value = 7;
  map.GetComponent(2).value = 9;
  map.GetComponent(3).value = 11;
  map.GetComponent(4).value = 13;

  assert(map.GetComponent(0).value == 5);
  assert(map.GetComponent(1).value == 7);
  assert(map.GetComponent(2).value == 9);
  assert(map.GetComponent(3).value == 11);
  assert(map.GetComponent(4).value == 13);

  map.RemoveComponent(1);
  map.RemoveComponent(3);

  assert(map.ContainsComponent(0));
  assert(!map.ContainsComponent(1));
  assert(map.ContainsComponent(2));
  assert(!map.ContainsComponent(3));
  assert(map.ContainsComponent(4));

  assert(map.GetComponent(0).value == 5);
  assert(map.GetComponent(2).value == 9);
  assert(map.GetComponent(4).value == 13);
}

/******************************************************************************/
inline void TestSceneSignatureManagement() {
  Scene scene;
  scene.RegisterComponentType<ComponentA>(100);
  scene.RegisterComponentType<ComponentB>(100);

  Signature sig;
  scene.AddComponentToSignature<ComponentA>(sig);
  auto systemA = scene.RegisterSystemType<SystemA>(sig);

  sig.reset();
  scene.AddComponentToSignature<ComponentB>(sig);
  auto systemB = scene.RegisterSystemType<SystemB>(sig);

  sig.reset();
  scene.AddComponentToSignature<ComponentA>(sig);
  scene.AddComponentToSignature<ComponentB>(sig);
  auto systemC = scene.RegisterSystemType<SystemC>(sig);

  auto entityA = scene.CreateEntity();
  auto entityB = scene.CreateEntity();
  auto entityC = scene.CreateEntity();
  auto entityD = scene.CreateEntity();
  auto entityE = scene.CreateEntity();

  assert(entityA == 0);
  assert(entityB == 1);
  assert(entityC == 2);
  assert(entityD == 3);
  assert(entityE == 4);

  scene.AddComponentToEntity<ComponentA>(entityA);
  scene.AddComponentToEntity<ComponentB>(entityB);
  scene.AddComponentToEntity<ComponentA>(entityC);
  scene.AddComponentToEntity<ComponentB>(entityC);

  assert(systemA->GetEntities().count(entityA));
  assert(!systemA->GetEntities().count(entityB));
  assert(systemA->GetEntities().count(entityC));
  assert(!systemA->GetEntities().count(entityD));
  assert(!systemA->GetEntities().count(entityE));

  assert(!systemB->GetEntities().count(entityA));
  assert(systemB->GetEntities().count(entityB));
  assert(systemB->GetEntities().count(entityC));
  assert(!systemB->GetEntities().count(entityD));
  assert(!systemB->GetEntities().count(entityE));

  assert(!systemC->GetEntities().count(entityA));
  assert(!systemC->GetEntities().count(entityB));
  assert(systemC->GetEntities().count(entityC));
  assert(!systemC->GetEntities().count(entityD));
  assert(!systemC->GetEntities().count(entityE));

  scene.RemoveComponentFromEntity<ComponentA>(entityA);

  assert(!systemA->GetEntities().count(entityA));
  assert(!systemA->GetEntities().count(entityB));
  assert(systemA->GetEntities().count(entityC));
  assert(!systemA->GetEntities().count(entityD));
  assert(!systemA->GetEntities().count(entityE));

  assert(!systemB->GetEntities().count(entityA));
  assert(systemB->GetEntities().count(entityB));
  assert(systemB->GetEntities().count(entityC));
  assert(!systemB->GetEntities().count(entityD));
  assert(!systemB->GetEntities().count(entityE));

  assert(!systemC->GetEntities().count(entityA));
  assert(!systemC->GetEntities().count(entityB));
  assert(systemC->GetEntities().count(entityC));
  assert(!systemC->GetEntities().count(entityD));
  assert(!systemC->GetEntities().count(entityE));

  scene.RemoveComponentFromEntity<ComponentB>(entityB);

  assert(!systemA->GetEntities().count(entityA));
  assert(!systemA->GetEntities().count(entityB));
  assert(systemA->GetEntities().count(entityC));
  assert(!systemA->GetEntities().count(entityD));
  assert(!systemA->GetEntities().count(entityE));

  assert(!systemB->GetEntities().count(entityA));
  assert(!systemB->GetEntities().count(entityB));
  assert(systemB->GetEntities().count(entityC));
  assert(!systemB->GetEntities().count(entityD));
  assert(!systemB->GetEntities().count(entityE));

  assert(!systemC->GetEntities().count(entityA));
  assert(!systemC->GetEntities().count(entityB));
  assert(systemC->GetEntities().count(entityC));
  assert(!systemC->GetEntities().count(entityD));
  assert(!systemC->GetEntities().count(entityE));

  scene.RemoveComponentFromEntity<ComponentA>(entityC);

  assert(!systemA->GetEntities().count(entityA));
  assert(!systemA->GetEntities().count(entityB));
  assert(!systemA->GetEntities().count(entityC));
  assert(!systemA->GetEntities().count(entityD));
  assert(!systemA->GetEntities().count(entityE));

  assert(!systemB->GetEntities().count(entityA));
  assert(!systemB->GetEntities().count(entityB));
  assert(systemB->GetEntities().count(entityC));
  assert(!systemB->GetEntities().count(entityD));
  assert(!systemB->GetEntities().count(entityE));

  assert(!systemC->GetEntities().count(entityA));
  assert(!systemC->GetEntities().count(entityB));
  assert(!systemC->GetEntities().count(entityC));
  assert(!systemC->GetEntities().count(entityD));
  assert(!systemC->GetEntities().count(entityE));

  scene.DestroyEntity(entityC);

  assert(!systemA->GetEntities().count(entityA));
  assert(!systemA->GetEntities().count(entityB));
  assert(!systemA->GetEntities().count(entityC));
  assert(!systemA->GetEntities().count(entityD));
  assert(!systemA->GetEntities().count(entityE));

  assert(!systemB->GetEntities().count(entityA));
  assert(!systemB->GetEntities().count(entityB));
  assert(!systemB->GetEntities().count(entityC));
  assert(!systemB->GetEntities().count(entityD));
  assert(!systemB->GetEntities().count(entityE));

  assert(!systemC->GetEntities().count(entityA));
  assert(!systemC->GetEntities().count(entityB));
  assert(!systemC->GetEntities().count(entityC));
  assert(!systemC->GetEntities().count(entityD));
  assert(!systemC->GetEntities().count(entityE));
}

/******************************************************************************/
inline void TestSceneEntityManagement() {
  Scene scene;
  scene.RegisterComponentType<ComponentA>(100);
  scene.RegisterComponentType<ComponentB>(100);

  auto entityA = scene.CreateEntity();
  auto entityB = scene.CreateEntity();
  auto entityC = scene.CreateEntity();
  auto entityD = scene.CreateEntity();
  auto entityE = scene.CreateEntity();

  assert(entityA == 0);
  assert(entityB == 1);
  assert(entityC == 2);
  assert(entityD == 3);
  assert(entityE == 4);

  scene.AddComponentToEntity<ComponentA>(entityA).value = 5;
  scene.AddComponentToEntity<ComponentB>(entityA).value = "five";
  scene.AddComponentToEntity<ComponentA>(entityB).value = 6;
  scene.AddComponentToEntity<ComponentB>(entityB).value = "six";
  scene.AddComponentToEntity<ComponentA>(entityC).value = 7;
  scene.AddComponentToEntity<ComponentB>(entityC).value = "seven";
  scene.AddComponentToEntity<ComponentA>(entityD).value = 8;
  scene.AddComponentToEntity<ComponentB>(entityD).value = "eight";
  scene.AddComponentToEntity<ComponentA>(entityE).value = 9;
  scene.AddComponentToEntity<ComponentB>(entityE).value = "nine";

  assert(scene.GetComponentForEntity<ComponentA>(entityA).value == 5);
  assert(scene.GetComponentForEntity<ComponentB>(entityA).value == "five");
  assert(scene.GetComponentForEntity<ComponentA>(entityB).value == 6);
  assert(scene.GetComponentForEntity<ComponentB>(entityB).value == "six");
  assert(scene.GetComponentForEntity<ComponentA>(entityC).value == 7);
  assert(scene.GetComponentForEntity<ComponentB>(entityC).value == "seven");
  assert(scene.GetComponentForEntity<ComponentA>(entityD).value == 8);
  assert(scene.GetComponentForEntity<ComponentB>(entityD).value == "eight");
  assert(scene.GetComponentForEntity<ComponentA>(entityE).value == 9);
  assert(scene.GetComponentForEntity<ComponentB>(entityE).value == "nine");

  scene.DestroyEntity(entityB);
  scene.DestroyEntity(entityD);

  assert(scene.GetComponentForEntity<ComponentA>(entityA).value == 5);
  assert(scene.GetComponentForEntity<ComponentB>(entityA).value == "five");
  assert(scene.GetComponentForEntity<ComponentA>(entityC).value == 7);
  assert(scene.GetComponentForEntity<ComponentB>(entityC).value == "seven");
  assert(scene.GetComponentForEntity<ComponentA>(entityE).value == 9);
  assert(scene.GetComponentForEntity<ComponentB>(entityE).value == "nine");

  entityB = scene.CreateEntity();

  scene.AddComponentToEntity<ComponentA>(entityB).value = 10;
  scene.AddComponentToEntity<ComponentB>(entityB).value = "ten";

  assert(scene.GetComponentForEntity<ComponentA>(entityB).value == 10);
  assert(scene.GetComponentForEntity<ComponentB>(entityB).value == "ten");
}

} // namespace KumaECS
