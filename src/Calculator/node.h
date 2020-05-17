#pragma once
#include <string>

enum class NodeType
{
    Value,
    Symbol,
    UnaryOp,
    BinaryOp
};

enum class OpType
{
    Assign,
    Negate,
    Plus,
    Minus,
    Mult,
    Div
};

struct Node
{
    NodeType type;
    int number; // if Value
    std::string name; // if Symbol
    OpType op_type; // if UnaryOp or BinaryOp
    Node* op1 = nullptr; // if UnaryOp or BinaryOp
    Node* op2 = nullptr; // if BinaryOp
};
