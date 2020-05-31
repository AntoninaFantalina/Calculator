#include <gtest/gtest.h>
#include "calculator.h"
#include "utils.h"

TEST(MinusTest, N_Minus_N) {
    // n - n
    std::vector<const Node*> nodes = calculate("3 - 3");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "0");
}

TEST(MinusTest, X_Minus_X) {
    // x - x
    std::vector<const Node*> nodes = calculate("x - x");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "0");
}

TEST(MinusTest, N_Minus_M) {
    // n - m
    std::vector<const Node*> nodes = calculate("23 - 8");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "15");
}

TEST(MinusTest, N_Minus_M_2) {
    // n - m
    std::vector<const Node*> nodes = calculate("23 - 40");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(-17)");
}

TEST(MinusTest, X_Plus_N_Minus_M) {
    // (X + n) - m -> X + (n - m)
    std::vector<const Node*> nodes = calculate("(x + 5) - 4");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x + 1)");
}

TEST(MinusTest, X_Plus_N_Minus_M_2) {
    // (X + n) - m -> X + (n - m)
    std::vector<const Node*> nodes = calculate("(5 + x) - 4");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x + 1)");
}

TEST(MinusTest, N_Minus_X) {
    // replace n - X -> (-X) + n
    std::vector<const Node*> nodes = calculate("12 - x");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "((-x) + 12)");
}

TEST(MinusTest, X_Minus_N_Minus_M) {
    // (X - n) - m -> X - (n + m)
    std::vector<const Node*> nodes = calculate("(x - 7) - 2");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x - 9)");
}

TEST(MinusTest, X_Mult_N_Minus_X) {
    // (X * n) - X -> X * (n - 1)
    std::vector<const Node*> nodes = calculate("(x * 5) - x");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x * 4)");
}

TEST(MinusTest, X_Mult_N_Minus_X_2) {
    // (n * X) - X -> X * (n - 1)
    std::vector<const Node*> nodes = calculate("(5 * x) - x");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x * 4)");
}

TEST(MinusTest, Neg_X1_Minus_X2) {
	// (-X1)-X2 -> -(X1+X2)
    std::vector<const Node*> nodes = calculate("(-x1) - x2");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(-(x1 + x2))");
}

TEST(MinusTest, Y_Minus_X_Minus_X) {
	// (Y - X) - X -> Y
    std::vector<const Node*> nodes = calculate("(y - x) - x");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "y");
}

TEST(MinusTest, X_Minus_Y_Minus_X) {
	// (X - Y) - X -> (-Y)
    std::vector<const Node*> nodes = calculate("(x - y) - x");
    ASSERT_EQ(nodes.size(), 1u);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(-y)");
}
