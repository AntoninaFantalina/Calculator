#include <iostream>
#include <vector>
#include "Calculator/node.h"
#include "Calculator/calculator.h"
#include "Calculator/utils.h"

int main() {
    std::vector<const Node*> nodes = calculate("x = 3; y = -(-x) + x + y + 4 + 123 + 7 * (-(5 + 14 / 7))");
    for (const auto node : nodes) {
        printExpression(*node);
        std::cout << std::endl;
    }
}
