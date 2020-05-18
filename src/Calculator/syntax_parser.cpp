#include <iostream>
#include <map>
#include <variant>
#include <vector>
#include "syntax_parser.h"
#include "utils.h"

namespace {
int opPriority(const OpType op_type) {
    if (op_type == OpType::Negate) {
        std::cout << " NEGATE ";
        return -1;
    }
    if (op_type == OpType::Assign) {
        std::cout << " ASSIGN ";
        return -1;
    }

    const std::map<OpType, int> priority_table 
    {
        {OpType::Plus, 1},
        {OpType::Minus, 1},
        {OpType::Mult, 2},
        {OpType::Div, 2}
    };
    
    return priority_table.at(op_type);
}

void error(const std::string& message) {
    std::cout << "ERROR " << message << std::endl;
}

}

SyntaxParser::SyntaxParser(Lexer& lexer)
    : lexer_(lexer) {
}

Node* SyntaxParser::parse() {
    return parseExpression();
}

Node* SyntaxParser::parseExpression(const bool lp_exist) {
    Node* lhs = parsePrimary();
    return parseBinaryOp(0, lhs, lp_exist);
}

Node* SyntaxParser::parsePrimary() {
    const auto [token, token_val] = lexer_.getToken();
    switch (token) {
    case Token::Name:
        return createNameNode(std::get<std::string>(*token_val));
    case Token::Number:
        return createValueNode(std::get<double>(*token_val));
    case Token::LP:
        return parseLParenthesis();
    default:
        error("parsePrimary: unexpected token " + tokenToString(token));
        return nullptr;
    }
}

Node* SyntaxParser::parseLParenthesis() {
    Node* expr = parseExpression(true);
    if (expr == nullptr) {
        return nullptr;
    }

    return expr;
}

Node* SyntaxParser::parseBinaryOp(int lhs_priority, Node* lhs, const bool lp_exist) {
    std::optional<Node*> node = getNextOp(lp_exist);
    if (!node.has_value()) {
        return lhs;
    }

    return parseBinaryOp(lhs_priority, lhs, node.value(), lp_exist);
}

Node* SyntaxParser::parseBinaryOp(int lhs_priority, Node* lhs, Node* node, const bool lp_exist) {
    if (lhs == nullptr) {
        return nullptr;
    }

    while (node) {
        const int priority = opPriority(node->op_type);
        
        if (priority < lhs_priority) {
            delete node;
            return lhs;
        }

        Node* rhs = parsePrimary();
        if (rhs == nullptr) {
            delete node;
            return lhs;
        }

        node->op1 = lhs;
        node->op2 = rhs;
        lhs = node;

        std::optional<Node*> next_node = getNextOp(lp_exist);
        if (!next_node.has_value() || next_node.value() == nullptr) {
            return node;
        }

        const int next_priority = opPriority(next_node.value()->op_type);
        if (priority < next_priority) {
            rhs = parseBinaryOp(priority, rhs, next_node.value(), lp_exist);
            if (rhs == nullptr) {
                delete node;
                return lhs;
            }
            node->op2 = rhs;

            next_node = getNextOp(lp_exist);
            if (!next_node.has_value()) {
                return node;
            }
        }
        node = next_node.value();
    }

    if (lp_exist) {
        error("parseBinaryOp: expected RP");
        return nullptr;
    }

    return lhs;
}

std::optional<Node*> SyntaxParser::getNextOp(const bool lp_exist) {
    const auto [next_token, next_token_val] = lexer_.getToken();
    if (next_token == Token::End) {
        if (lp_exist) {
            error("parseBinaryOp: expected RP");
        }
        return std::nullopt;
    }

    if (lp_exist && next_token == Token::RP) {
        return std::nullopt;
    }

    return createBinaryOpNode(next_token);
}

Node* SyntaxParser::createValueNode(const double value) const {
    Node* node = new Node();
    node->type = NodeType::Value;
    node->number = value;
    return node;
}

Node* SyntaxParser::createNameNode(const std::string& name) const {
    Node* node = new Node();
    node->type = NodeType::Symbol;
    node->name = name;
    return node;
}

Node* SyntaxParser::createBinaryOpNode(const Token token) {
    switch (token) {
    case Token::Plus:
        return createOpNode(OpType::Plus);
    case Token::Minus:
        return createOpNode(OpType::Minus);
    case Token::Mult:
        return createOpNode(OpType::Mult);
    case Token::Div:
        return createOpNode(OpType::Div);
    default:
        error("createBinaryOpNode: unexpected token " + tokenToString(token));
        return nullptr;
    }
}

Node* SyntaxParser::createOpNode(const OpType op_type) const {
    Node* node = new Node();
    node->type = op_type == OpType::Negate ? NodeType::UnaryOp : NodeType::BinaryOp;
    node->op_type = op_type;
    return node;
}
