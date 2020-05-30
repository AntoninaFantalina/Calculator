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
    NodeType type_;
    int64_t number_ = 0; // if Number
    std::string name_; // if Name

    OpType op_type_; // if UnaryOp or BinaryOp
    const Node* op1_ = nullptr; // if UnaryOp or BinaryOp
    const Node* op2_ = nullptr; // if BinaryOp

	const Node** binded_node_ = nullptr; // if NodePtrToNode
	int64_t* binded_number_ = nullptr; // if NodePtrToNumber

    Node(const NodeType type);
    Node(const OpType op_type, const Node* op1 = nullptr, const Node* op2 = nullptr);
};

Node createNumberNode(const int64_t number);
Node createNameNode(const std::string& name);
