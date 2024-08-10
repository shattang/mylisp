#pragma once

#include <vector>
#include <string>

namespace Shattang::MyLisp
{
    enum class TokenType
    {
        OPEN_PAREN,  // (
        CLOSE_PAREN, // )
        SYMBOL,  // starts with alphabet can contain alphanum, - , _ and ?
        FLOAT,   // floating point number
        INTEGER, // integer number
        BOOL_TRUE,  // true
        BOOL_FALSE, // false
        STRING, // a quoted "string"
        END_OF_FILE, // EOF
        ERROR // To indicate invalid tokens
    };

    struct Token
    {
        TokenType type_;
        std::string_view value_;
        int line_;
        int column_;
        std::string error_;

        std::string ToString() const;
    };

    std::string TokenTypeToString(TokenType type);

    class Lexer
    {
    public:
        Lexer(const std::string &input)
            : input_(input), index_(0), line_(1), column_(1) {}

        Token GetNextToken();

    private:
        const std::string &input_; // Class holds a reference, so caller must hold the input
        std::size_t index_;        // The index where next token parsing will begin
        int line_;                 // Tracks the current line number
        int column_;               // Tracks the current column number

        void handleWhitespace();
        void handleComment();
        Token makeToken(TokenType type);
        Token makeErrorToken(const std::string &errorMessage);
        Token makeEOFToken();
        Token handleNumber();
        Token handleAlpha();
        Token tokenizeFloat(size_t start);
        Token tokenizeInteger(size_t start);
        Token tokenizeString();
    };

    std::vector<Token> Tokenize(const std::string &input);

}