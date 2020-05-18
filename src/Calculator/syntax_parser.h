#pragma once
#include "lexer.h"
#include "node.h"

class SyntaxParser
{
public:
    SyntaxParser(Lexer& lexer);

    Node* parse();

private:
    Node* parseExpression(const bool lp_exist = false);
    Node* parsePrimary();
    Node* parseLParenthesis();
    Node* parseBinaryOp(int lhs_priority, Node* lhs, const bool lp_exist = false);
    Node* parseBinaryOp(int lhs_priority, Node* lhs, Node* node, const bool lp_exist = false);
    std::optional<Node*> getNextOp(const bool lp_exist);

    Node* createValueNode(const double value) const;
    Node* createNameNode(const std::string& name) const;
    Node* createBinaryOpNode(const Token token);
    Node* createOpNode(const OpType op_type) const;

private:
    Lexer& lexer_;
};
