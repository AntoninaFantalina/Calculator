#pragma once
#include <optional>
#include <string>
#include <variant>

enum class Token
{
    Name, Number, End,
    Plus='+', Minus='-', Mult='*', Div='/',
    Print=';', Assign='=', LP='(', RP=')'
};

using TokenValue = std::variant<double, std::string>;

class Lexer
{
public:
    Lexer(const std::string& input);

    std::pair<Token, std::optional<TokenValue>> getToken();

private:
    const std::string& source_;
    const std::string::const_iterator begin_;
    const std::string::const_iterator end_;
    std::string::const_iterator head_;
};
