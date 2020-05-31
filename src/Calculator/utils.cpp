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
    return "";
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
    return "";
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
    return "";
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

std::string convertExpression(const Node& node) {
    std::string expression = "";
    switch (node.type_) {
    case NodeType::Number:
        expression.append(std::to_string(node.number_));
        break;
    case NodeType::Name:
        expression.append(node.name_);
        break;
    case NodeType::UnaryOp:
        expression.append("(" + opTypeToString(node.op_type_) + convertExpression(*node.op1_) + ")");
        break;
    case NodeType::BinaryOp:
        expression.append("(" + convertExpression(*node.op1_) +
                          " " + opTypeToString(node.op_type_) +
                          " " + convertExpression(*node.op2_) + ")");
        break;
	default:
		assert(0);
    }
    return expression;
}
