#pragma once
#include <string>
#include "node.h"

class Calculator
{
public:
    Node* compile(const std::string& input);
};
