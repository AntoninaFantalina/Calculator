#include "calculator.h"
#include "node.h"
#include "lexer.h"
#include "syntax_parser.h"

std::vector<Node*> calculate(const std::string& input) {
    Lexer lexer(input);
    SyntaxParser parser(lexer);
    return parser.parse();
}
