#include <gtest/gtest.h>
#include "calculator.h"
#include "utils.h"

TEST(NegateTest, Neg_Neg_X) {
    // (-(-X)) -> X
    std::vector<const Node*> nodes = calculate("-(-x)");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "x");
}

TEST(NegateTest, Neg_Neg_Neg_X) {
    // (-(-(-X))) -> (-X)
    std::vector<const Node*> nodes = calculate("-(-(-x))");
    ASSERT_EQ(nodes.size(), 1);
    ASSERT_TRUE(nodes.front() != nullptr);
    ASSERT_EQ(convertExpression(*nodes.front()), "(-x)");
}
