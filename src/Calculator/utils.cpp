#include <iostream>
#include "utils.h"

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
    case NodeType::Number:
        return "Number";
    case NodeType::Name:
        return "Name";
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

void printTree(const Node& node, const std::string& indent) {
    std::cout << indent << "type = " << nodeTypeToString(node.type_);
    switch (node.type_) {
    case NodeType::Number:
        std::cout << ", number = " << std::to_string(node.number_);
        break;
    case NodeType::Name:
        std::cout << ", name = " << node.name_;
        break;
    case NodeType::UnaryOp:
    case NodeType::BinaryOp:
        std::cout << ", op_type = " << opTypeToString(node.op_type_);
        break;
	default:
		assert(0);
    }
    std::cout << std::endl;

    if (node.op1_) {
        printTree(*node.op1_, indent + "  ");
    }
    if (node.op2_) {
        printTree(*node.op2_, indent + "  ");
    }
}

void printExpression(const Node& node) {
    switch (node.type_) {
    case NodeType::Number:
        std::cout << std::to_string(node.number_);
        break;
    case NodeType::Name:
        std::cout << node.name_;
        break;
    case NodeType::UnaryOp:
        std::cout << "(";
        std::cout << opTypeToString(node.op_type_);
        printExpression(*node.op1_);
        std::cout << ")";
        break;
    case NodeType::BinaryOp:
        std::cout << "(";
        printExpression(*node.op1_);
        std::cout << " " << opTypeToString(node.op_type_) << " ";
        printExpression(*node.op2_);
        std::cout << ")";
        break;
	default:
		assert(0);
    }
}
