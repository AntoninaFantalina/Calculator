#include "calculator.h"
#include "node.h"
#include "lexer.h"
#include "syntax_parser.h"
#include "postprocessor.h"

std::vector<Node*> calculate(const std::string& input) {
    Lexer lexer(input);
    SyntaxParser parser(lexer);
    std::vector<Node*> nodes = parser.parse();
    return reduce(nodes);
}
