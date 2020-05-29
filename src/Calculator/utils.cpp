#include <iostream>
#include "utils.h"

namespace
{

void printExpressionRecursive(const Node* node) {
    if (node == nullptr) {
        return;
    }
    
    switch (node->type) {
    case NodeType::Value:
        std::cout << std::to_string(node->number);
        break;
    case NodeType::Symbol:
        std::cout << node->name;
        break;
    case NodeType::UnaryOp:
        std::cout << "(";
        std::cout << opTypeToString(node->op_type);
        printExpressionRecursive(node->op1);
        std::cout << ")";
        break;
    case NodeType::BinaryOp:
        std::cout << "(";
        printExpressionRecursive(node->op1);
        std::cout << " " << opTypeToString(node->op_type) << " ";
        printExpressionRecursive(node->op2);
        std::cout << ")";
        break;
	default:
		assert(0);
    }
}

} // namespace

std::string tokenToString(const Token token) {
    switch (token) {
    case Token::Name:
        return "Name";
    case Token::Number:
        return "Number";
    case Token::End:
        return "End";
    case Token::Plus:
        return "Plus";
    case Token::Minus:
        return "Minus";
    case Token::Mult:
        return "Mult";
    case Token::Div:
        return "Div";
    case Token::Print:
        return "Print";
    case Token::Assign:
        return "Assign";
    case Token::LP:
        return "LP";
    case Token::RP:
        return "RP";
	default:
		assert(0);
    }
}

std::string nodeTypeToString(const NodeType type) {
    switch (type) {
    case NodeType::Value:
        return "Value";
    case NodeType::Symbol:
        return "Symbol";
    case NodeType::UnaryOp:
        return "UnaryOp";
    case NodeType::BinaryOp:
        return "BinaryOp";
	default:
		assert(0);
    }
}

std::string opTypeToString(const OpType type) {
    switch (type) {
    case OpType::Assign:
        return "=";
    case OpType::Negate:
        return "-";
    case OpType::Plus:
        return "+";
    case OpType::Minus:
        return "-";
    case OpType::Mult:
        return "*";
    case OpType::Div:
        return "/";
	default:
		assert(0);
    }
}

void printTree(const Node* node, const std::string& indent) {
    if (node == nullptr) {
        return;
    }
    
    std::cout << indent << "type = " << nodeTypeToString(node->type);
    switch (node->type) {
    case NodeType::Value:
        std::cout << ", number = " << std::to_string(node->number);
        break;
    case NodeType::Symbol:
        std::cout << ", name = " << node->name;
        break;
    case NodeType::UnaryOp:
    case NodeType::BinaryOp:
        std::cout << ", op_type = " << opTypeToString(node->op_type);
        break;
	default:
		assert(0);
    }
    std::cout << std::endl;

    printTree(node->op1, indent + "  ");
    printTree(node->op2, indent + "  ");
}

void printExpression(const Node* node) {
    printExpressionRecursive(node);
    std::cout << std::endl;
}
