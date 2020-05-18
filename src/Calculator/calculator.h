#pragma once
#include <string>
#include <vector>
#include "node.h"

class Calculator
{
public:
    std::vector<Node*> compile(const std::string& input);
};
