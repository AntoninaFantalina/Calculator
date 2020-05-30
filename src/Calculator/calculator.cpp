#include "calculator.h"
#include "node.h"
#include "lexer.h"
#include "syntax_parser.h"
#include "postprocessor.h"

std::vector<const Node*> calculate(const std::string& input) {
    Lexer lexer(input);
    SyntaxParser parser(lexer);
    std::vector<const Node*> nodes = parser.parse();
    PostProcessor postprocessor;
    return postprocessor.reduce(nodes);
}
