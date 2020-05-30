#pragma once
#include <vector>
#include "node.h"
#include "calculator.h"

class PostProcessor
{
public:
    std::vector<const Node*> reduce(std::vector<const Node*> nodes);

private:
    Node reduceStep(const Node& node);
    Node reduceMult(const Node& node);
    Node reduceDiv(const Node& node);
    Node reduceAdd(const Node& node);
    Node reduceNeg(const Node& node);

private:
    bool reduced_something_ = false;
};
