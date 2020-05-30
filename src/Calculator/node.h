#pragma once
#include <string>

enum class NodeType
{
    Number,
    Name,
    UnaryOp,
    BinaryOp,
    NodePtrToNode,
    NodePtrToNumber
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
    int64_t number; // if Number
    std::string name; // if Name
    OpType op_type; // if UnaryOp or BinaryOp
    Node* op1 = nullptr; // if UnaryOp or BinaryOp
    Node* op2 = nullptr; // if BinaryOp

	Node** binded_node; // if NodePtrToNode
	int64_t* binded_number; // if NodePtrToNumber
};

Node* createNumberNode(const int64_t number);
Node* createNameNode(const std::string& name);
Node* createOpNode(const OpType op_type);
Node* createUnaryOpNode(const OpType op_type, Node* op1 = nullptr);
Node* createBinaryOpNode(const OpType op_type, Node* op1 = nullptr, Node* op2 = nullptr);
