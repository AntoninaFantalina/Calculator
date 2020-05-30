#include "node.h"

Node::Node(const NodeType type) {
    type_ = type;
}

Node::Node(const OpType op_type, const Node* op1, const Node* op2) {
    type_ = op_type == OpType::Negate ? NodeType::UnaryOp : NodeType::BinaryOp;
    op_type_ = op_type;
    op1_ = op1;
    op2_ = op2;
}

Node createNumberNode(const int64_t number) {
    Node node (NodeType::Number);
    node.number_ = number;
    return node;
}

Node createNameNode(const std::string& name) {
    Node node (NodeType::Name);
    node.name_ = name;
    return node;
}
