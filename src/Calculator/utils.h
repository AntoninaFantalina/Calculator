#pragma once
#include "lexer.h"
#include "node.h"

std::string tokenToString(const Token token);

std::string nodeTypeToString(const NodeType type);

std::string opTypeToString(const OpType type);

void printTree(Node* root, const std::string& indent = "");
