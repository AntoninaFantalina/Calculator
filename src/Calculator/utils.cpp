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
    }
}

std::string opTypeToString(const OpType type) {
    switch (type) {
    case OpType::Assign:
        return "Assign";
    case OpType::Negate:
        return "Negate";
    case OpType::Plus:
        return "Plus";
    case OpType::Minus:
        return "Minus";
    case OpType::Mult:
        return "Mult";
    case OpType::Div:
        return "Div";
    }
}

void printTree(const Node* node, const std::string& indent) {
    if (node == nullptr) {
        return;
    }
    
    std::string message = indent;
    message += "type = " + nodeTypeToString(node->type);
    switch (node->type) {
    case NodeType::Value:
        message += ", number = " + std::to_string(node->number);
        break;
    case NodeType::Symbol:
        message += ", name = " + node->name;
        break;
    default:
        message += ", op_type = " + opTypeToString(node->op_type);
    }
    std::cout << message << std::endl;

    printTree(node->op1, indent + " ");
    printTree(node->op2, indent + " ");
}
