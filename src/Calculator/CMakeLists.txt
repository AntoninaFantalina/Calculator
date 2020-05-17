set(MODULE_NAME CalculatorLib)

add_library(${MODULE_NAME}
  calculator.cpp
  calculator.h
  lexer.cpp
  lexer.h
  node.h
  syntax_parser.cpp
  syntax_parser.h
  utils.cpp
  utils.h
)
