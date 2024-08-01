#pragma once

#include <vector>
#include <string>

namespace Shattang::MyLisp
{
    enum class TokenType
    {
        OPEN_PAREN,
        CLOSE_PAREN,
        QUOTE,
        SYMBOL,
        NUMBER,
        STRING,
        LT,
        GT,
        EQ,
        LEQ,
        GEQ,
        NEQ,
        PLUS,
        MINUS,
        STAR,
        SLASH,
        PERCENT,
        // Add more token types for other MyLisp constructs as needed
        ERROR // To indicate invalid tokens
    };

    std::string TokenTypeToString(TokenType type);

    struct Token
    {
        TokenType type;
        std::string value;
        int line;
        int column;

        std::string ToString() const;
    };

    class Lexer
    {
    public:
        std::vector<Token> Tokenize(const std::string &input);
    };

}