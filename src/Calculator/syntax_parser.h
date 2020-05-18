#pragma once
#include <map>
#include <optional>
#include <vector>
#include "lexer.h"
#include "node.h"

class SyntaxParser
{
public:
    SyntaxParser(Lexer& lexer);

    std::vector<Node*> parse();

private:
    Node* parseExpression();
    Node* parsePrimary();
    Node* parseLParenthesis();
    Node* parseBinaryOp(int lhs_priority, Node* lhs);
    Node* parseBinaryOp(int lhs_priority, Node* lhs, Node* node);
    std::optional<Node*> getNextOp();

    Node* createValueNode(const double value) const;
    Node* createNameNode(const std::string& name) const;
    Node* createBinaryOpNode(const Token token);
    Node* createOpNode(const OpType op_type) const;

    int opPriority(const OpType op_type) const;
    void error(const std::string& message) const;

private:
    Lexer& lexer_;
    static std::map<OpType, int> priority_table_;
    size_t lp_count_ = 0;
};
