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

    std::vector<const Node*> parse();

private:
    Node* parseExpression();
    Node* parsePrimary();
    Node* parseLParenthesis();
    Node* parseBinaryOp(int lhs_priority, Node* lhs);
    Node* parseBinaryOp(int lhs_priority, Node* lhs, Node* node);
    std::optional<Node*> getNextOp();

    Node* createBinaryOpNode(const Token token);

    int opPriority(const OpType op_type) const;
    void error(const std::string& message) const;

private:
    Lexer& lexer_;
    static std::map<OpType, int> priority_table_;
    size_t lp_count_ = 0;
};
