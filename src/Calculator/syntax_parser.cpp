#include <iostream>
#include <variant>
#include "syntax_parser.h"
#include "utils.h"

std::map<OpType, int> SyntaxParser::priority_table_
{
    {OpType::Assign, 1},
    {OpType::Plus, 2},
    {OpType::Minus, 2},
    {OpType::Mult, 3},
    {OpType::Div, 3}
};

SyntaxParser::SyntaxParser(Lexer& lexer)
    : lexer_(lexer) {
}

std::vector<const Node*> SyntaxParser::parse() {
    std::vector<const Node*> nodes;
    while (const Node* node = parseExpression()) {
        nodes.push_back(node);
    }
    return nodes;
}

Node* SyntaxParser::parseExpression() {
    Node* lhs = parsePrimary();
    return parseBinaryOp(0, lhs);
}

Node* SyntaxParser::parsePrimary() {
    const auto [token, token_val] = lexer_.getToken();
    switch (token) {
    case Token::Minus: {
        return new Node(OpType::Negate, parsePrimary());
    }
    case Token::Name:
        return new Node(createNameNode(std::get<std::string>(*token_val)));
    case Token::Number:
        return new Node(createNumberNode(std::get<int64_t>(*token_val)));
    case Token::LP:
        return parseLParenthesis();
    case Token::Print:
    case Token::End:
        return nullptr;
    default:
        error("unexpected token " + tokenToString(token));
        return nullptr;
    }
}

Node* SyntaxParser::parseLParenthesis() {
    ++lp_count_;
    Node* expr = parseExpression();
    if (expr == nullptr) {
        return nullptr;
    }

    return expr;
}

Node* SyntaxParser::parseBinaryOp(int lhs_priority, Node* lhs) {
    std::optional<Node*> node = getNextOp();
    if (!node.has_value()) {
        return lhs;
    }

    return parseBinaryOp(lhs_priority, lhs, node.value());
}

Node* SyntaxParser::parseBinaryOp(int lhs_priority, Node* lhs, Node* node) {
    if (lhs == nullptr) {
        return nullptr;
    }

    while (node) {
        const int priority = opPriority(node->op_type_);
        
        if (priority < lhs_priority) {
            delete node;
            return lhs;
        }

        Node* rhs = parsePrimary();
        if (rhs == nullptr) {
            delete node;
            return lhs;
        }

        node->op1_ = lhs;
        node->op2_ = rhs;
        lhs = node;

        std::optional<Node*> next_node = getNextOp();
        if (!next_node.has_value() || next_node.value() == nullptr) {
            return node;
        }

        const int next_priority = opPriority(next_node.value()->op_type_);
        if (priority < next_priority) {
            rhs = parseBinaryOp(priority, rhs, next_node.value());
            if (rhs == nullptr) {
                delete node;
                return lhs;
            }
            node->op2_ = rhs;

            next_node = getNextOp();
            if (!next_node.has_value()) {
                return node;
            }
        }
        node = next_node.value();
    }

    if (lp_count_ > 0) {
        error("expected RP");
        return nullptr;
    }

    return lhs;
}

std::optional<Node*> SyntaxParser::getNextOp() {
    const auto [next_token, next_token_val] = lexer_.getToken();

    if (next_token == Token::RP && lp_count_ > 0) {
        --lp_count_;
        return std::nullopt;
    }

    if (next_token == Token::End || next_token == Token::Print) {
        if (lp_count_ != 0) {
            error("expected RP");
        }
        return std::nullopt;
    }

    return createBinaryOpNode(next_token);
}

Node* SyntaxParser::createBinaryOpNode(const Token token) {
    switch (token) {
    case Token::Plus:
        return new Node(OpType::Plus);
    case Token::Minus:
        return new Node(OpType::Minus);
    case Token::Mult:
        return new Node(OpType::Mult);
    case Token::Div:
        return new Node(OpType::Div);
    case Token::Assign:
        return new Node(OpType::Assign);
    default:
        error("unexpected token " + tokenToString(token));
        return nullptr;
    }
}

int SyntaxParser::opPriority(const OpType op_type) const {
    if (op_type == OpType::Negate) {
        error("unexpected op_type " + opTypeToString(op_type));
        return -1;
    }

    return priority_table_.at(op_type);
}

void SyntaxParser::error(const std::string& message) const {
    std::cout << lexer_.getCurrentString() << std::endl;
    std::cout << "ERROR: " << message << std::endl;
}
