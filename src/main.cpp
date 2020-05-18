#include <iostream>
#include <vector>
#include "Calculator/calculator.h"
#include "Calculator/utils.h"

int main() {
    Calculator calc;
    std::vector<Node*> nodes = calc.compile("x = 3; y = -x + 123 * (-(5 + 9 / 7));");
    for (const auto node : nodes) {
        printTree(node);
        std::cout << std::endl;
    }
}
