#include "calculator.h"
#include "lexer.h"
#include "syntax_parser.h"

std::vector<Node*> Calculator::compile(const std::string& input) {
    Lexer lexer(input);
    SyntaxParser parser(lexer);
    return parser.parse();
}
