#include <gtest/gtest.h>
#include "calculator.h"
#include "utils.h"

TEST(DivTest, N_Div_N) {
    // n / n -> 1
    std::vector<const Node*> nodes = calculate("17 / 17");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "1");
}

TEST(DivTest, X_Div_X) {
    // x / x -> 1
    std::vector<const Node*> nodes = calculate("x / x");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "1");
}

TEST(DivTest, N_Div_M) {
    // n / m -> k
    std::vector<const Node*> nodes = calculate("12 / 3");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "4");
}

TEST(DivTest, N_Div_M_2) {
    // n / m
    std::vector<const Node*> nodes = calculate("12 / 5");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(12 / 5)");
}

TEST(DivTest, X_Div_1) {
    // X / 1 -> X
    std::vector<const Node*> nodes = calculate("x / 1");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "x");
}

TEST(DivTest, X_Div_N_Div_M) {
    // (X / n) / m -> X / (n * m)
    std::vector<const Node*> nodes = calculate("(x / 5) / 2");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x / 10)");
}

TEST(DivTest, N_Div_X_Div_M) {
    // (n / X) / m -> (n / m) / X
    std::vector<const Node*> nodes = calculate("(6 / x) / 2");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(3 / x)");
}
