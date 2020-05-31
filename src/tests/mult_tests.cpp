#include <gtest/gtest.h>
#include "calculator.h"
#include "utils.h"

TEST(MultTest, N_Mult_N) {
    // n * n
    std::vector<const Node*> nodes = calculate("3 * 3");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "9");
}

TEST(MultTest, N_Mult_M) {
    // n * m
    std::vector<const Node*> nodes = calculate("10 * 14");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "140");
}

TEST(MultTest, N_Mult_X) {
    // replace n * X -> X * n
    std::vector<const Node*> nodes = calculate("8 * x");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x * 8)");
}

TEST(MultTest, N_Mult_0) {
    // n * 0 -> 0
    std::vector<const Node*> nodes = calculate("3 * 0");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "0");
}

TEST(MultTest, X_Mult_0) {
    // X * 0 -> 0
    std::vector<const Node*> nodes = calculate("x * 0");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "0");
}

TEST(MultTest, N_Mult_1) {
    // n * 1 -> n
    std::vector<const Node*> nodes = calculate("3 * 1");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "3");
}

TEST(MultTest, X_Mult_1) {
    // X * 1 -> X
    std::vector<const Node*> nodes = calculate("x * 1");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "x");
}

TEST(MultTest, X_Mult_N_Mult_M) {
    // (X * n) * m -> X * (n * m)
    std::vector<const Node*> nodes = calculate("(x * 5) * 7");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x * 35)");
}

TEST(MultTest, X_Mult_N_Mult_M_2) {
    // (n * X) * m -> X * (n * m)
    std::vector<const Node*> nodes = calculate("(5 * x) * 7");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x * 35)");
}

TEST(MultTest, X1_Mult_Neg_X2) {
    // X1 * (-X2) -> (-(X1 * X2))
    std::vector<const Node*> nodes = calculate("x1 * (-x2)");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(-(x1 * x2))");
}

TEST(MultTest, Neg_X1_Mult_X2_2) {
    // (-X1) * X2 -> (-(X1 * X2))
    std::vector<const Node*> nodes = calculate("(-x1) * x2");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(-(x1 * x2))");
}

TEST(MultTest, Neg_X1_Mul_Neg_X2) {
    // (-X1) * (-X2) -> X1 * X2
    std::vector<const Node*> nodes = calculate("(-x1) * (-x2)");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x1 * x2)");
}

TEST(MultTest, N_Div_M_Mult_M) {
    // (n / X) * X -> n
    std::vector<const Node*> nodes = calculate("(3 / 7) * 7");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "3");
}

TEST(MultTest, N_Div_X_Mult_X) {
    // (n / X) * X -> n
    std::vector<const Node*> nodes = calculate("(3 / x) * x");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "3");
}
