#include <gtest/gtest.h>

#include "KumaECS/ComponentMap.hpp"

namespace KumaECS
{
  struct TestComponent
  {
    int x{0};
  };

  TEST(ComponentMapTests, AddComponent)
  {
    ComponentMap<TestComponent> map(10);
    map.AddComponent(0).x = 0;
    map.AddComponent(1).x = 1;
    map.AddComponent(2).x = 2;
    map.AddComponent(3).x = 3;

    EXPECT_TRUE(map.ContainsComponent(0));
    EXPECT_TRUE(map.ContainsComponent(1));
    EXPECT_TRUE(map.ContainsComponent(2));
    EXPECT_TRUE(map.ContainsComponent(3));

    EXPECT_FALSE(map.ContainsComponent(4));

    EXPECT_EQ(map.GetComponent(0).x, 0);
    EXPECT_EQ(map.GetComponent(1).x, 1);
    EXPECT_EQ(map.GetComponent(2).x, 2);
    EXPECT_EQ(map.GetComponent(3).x, 3);
  }

  TEST(ComponentMapTests, RemoveComponent)
  {
    ComponentMap<TestComponent> map(10);
    map.AddComponent(0).x = 0;
    map.AddComponent(1).x = 1;
    map.AddComponent(2).x = 2;
    map.AddComponent(3).x = 3;
    map.AddComponent(4).x = 4;
    map.RemoveComponent(1);
    map.RemoveComponent(3);

    EXPECT_TRUE(map.ContainsComponent(0));
    EXPECT_TRUE(map.ContainsComponent(2));
    EXPECT_TRUE(map.ContainsComponent(4));

    EXPECT_FALSE(map.ContainsComponent(1));
    EXPECT_FALSE(map.ContainsComponent(3));

    EXPECT_EQ(map.GetComponent(0).x, 0);
    EXPECT_EQ(map.GetComponent(2).x, 2);
    EXPECT_EQ(map.GetComponent(4).x, 4);
  }
} // namespace KumaECS