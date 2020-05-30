set(MODULE_NAME CalculatorLib)

add_library(${MODULE_NAME}
  calculator.cpp
  calculator.h
  lexer.cpp
  lexer.h
  node.cpp
  node.h
  postprocessor.cpp
  postprocessor.h
  syntax_parser.cpp
  syntax_parser.h
  utils.cpp
  utils.h
)
