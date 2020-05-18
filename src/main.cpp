#include <iostream>
#include "Calculator/calculator.h"
#include "Calculator/utils.h"

int main() {
  Calculator calc;
  Node* root = calc.compile("-x + 123 * (-(5 / 9 + 7))");
  printTree(root);
  std::cout << "Hello World!\n";
}
