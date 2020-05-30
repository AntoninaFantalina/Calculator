#include "node.h"

Node* createNumberNode(const int64_t number) {
    Node* node = new Node();
    node->type = NodeType::Number;
    node->number = number;
    return node;
}

Node* createNameNode(const std::string& name) {
    Node* node = new Node();
    node->type = NodeType::Name;
    node->name = name;
    return node;
}

Node* createOpNode(const OpType op_type) {
    Node* node = new Node();
    node->type = op_type == OpType::Negate ? NodeType::UnaryOp : NodeType::BinaryOp;
    node->op_type = op_type;
    return node;
}

Node* createUnaryOpNode(const OpType op_type, Node* op1) {
	Node* node = createOpNode(op_type);
	node->op1 = op1;
    return node;
}

Node* createBinaryOpNode(const OpType op_type, Node* op1, Node* op2) {
	Node* node = createOpNode(op_type);
	node->op1 = op1;
	node->op2 = op2;
    return node;
}
