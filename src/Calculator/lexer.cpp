#include <sstream>
#include "lexer.h"

Lexer::Lexer(const std::string& input) 
    : source_(input)
    , begin_(input.begin())
    , end_(input.end())
    , head_(input.begin()) {
}

std::pair<Token, std::optional<TokenValue>> Lexer::getToken() {
    while (head_ != end_ && isspace(*head_)) {
        ++head_;
    }
    if (head_ == end_) {
        return {Token::End, std::nullopt};
    }

    const char ch = *head_;
    ++head_;
    switch (ch)
    {
        case 0:
            return {Token::End, std::nullopt};
        case ';':
        case '\n':
            return {Token::Print, std::nullopt};
        case '*': case '/': case '+': case '-': case '(': case ')': case '=':
            return {Token(ch), std::nullopt};
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': case '.': {
            const auto& start = std::prev(head_);
            while (head_ != end_ && isdigit(*head_)) {
                ++head_;
            }
            const double number_value = std::stod(std::string{start, head_});
            return {Token::Number, number_value};
        }
        default:
            if (isalpha(ch))
            {
                const auto& start = std::prev(head_);
                while (head_ != end_ && isalnum(*head_)) {
                    ++head_;
                }
                return {Token::Name, std::string{start, head_}};
            }
            // error
            return {Token::Print, std::nullopt};
    }
}
