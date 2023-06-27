#include "Tests.hpp"

#include <iostream>

int main() {
  std::cout << "Testing ComponentMap..." << std::endl;
  KumaECS::TestComponentMap();
  std::cout << "ComponentMap tests successful!" << std::endl;

  std::cout << std::endl;

  std::cout << "Testing Scene signature management..." << std::endl;
  KumaECS::TestSceneSignatureManagement();
  std::cout << "Scene signature management successful!" << std::endl;

  std::cout << std::endl;

  std::cout << "Testing Scene entity management..." << std::endl;
  KumaECS::TestSceneEntityManagement();
  std::cout << "Scene entity management successful!" << std::endl;

  std::cout << std::endl;

  return 0;
}
