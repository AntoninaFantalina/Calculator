#include <gtest/gtest.h>
#include "calculator.h"
#include "utils.h"

TEST(PlusTest, N_Plus_N) {
    // n + n
    std::vector<const Node*> nodes = calculate("3 + 3");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "6");
}

TEST(PlusTest, N_Plus_M) {
    // n + m
    std::vector<const Node*> nodes = calculate("3 + 7");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "10");
}

TEST(PlusTest, N_Plus_X) {
    // replace n + X -> X + n
    std::vector<const Node*> nodes = calculate("3 + x");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x + 3)");
}

TEST(PlusTest, X_Plus_N_Plus_M) {
    // (X + n) + m -> X + k
    std::vector<const Node*> nodes = calculate("(x + 5) + 4");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x + 9)");
}

TEST(PlusTest, X_Plus_N_Plus_M_2) {
    // (n + X) + m -> X + k
    std::vector<const Node*> nodes = calculate("(5 + x) + 4");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x + 9)");
}

TEST(PlusTest, X_Mult_N_Plus_X) {
    // (X * n) + X -> X * (n + 1)
    std::vector<const Node*> nodes = calculate("(x * 5) + x");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x * 6)");
}

TEST(PlusTest, X_Mult_N_Plus_X_2) {
    // (n * X) + X -> X * (n + 1)
    std::vector<const Node*> nodes = calculate("(5 * x) + x");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x * 6)");
}

TEST(PlusTest, X1_Plus_Neg_X2) {
	// X1+(-X2) -> X1-X2
    std::vector<const Node*> nodes = calculate("x1 + (-x2)");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x1 - x2)");
}

TEST(PlusTest, X_Mult_N_Plus_X_Mult_M) {
    // (X*n)+(X*m) -> X*(n+m)
    std::vector<const Node*> nodes = calculate("(x * 3) + (x * 5)");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(x * 8)");
}
