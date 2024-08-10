#include <Shattang/MyLisp/Lexer.h>

#include <cctype>
#include <sstream>

namespace Shattang::MyLisp
{
    std::string TokenTypeToString(TokenType type)
    {
        switch (type)
        {
        case TokenType::OPEN_PAREN:
            return "OPEN_PAREN";
        case TokenType::CLOSE_PAREN:
            return "CLOSE_PAREN";
        case TokenType::SYMBOL:
            return "SYMBOL";
        case TokenType::FLOAT:
            return "FLOAT";
        case TokenType::INTEGER:
            return "INTEGER";
        case TokenType::BOOL_FALSE:
            return "BOOL_FALSE";
        case TokenType::BOOL_TRUE:
            return "BOOL_TRUE";
        case TokenType::STRING:
            return "STRING";
        case TokenType::ERROR:
            return "ERROR";
        case TokenType::END_OF_FILE:
            return "EOF";
        // Add cases for other token types
        default:
            return "UNKNOWN";
        }
    }

    std::string Token::ToString() const
    {
        std::ostringstream oss;
        oss << "Type: " << TokenTypeToString(type_)
            << ", Value: " << value_
            << ", Line: " << line_
            << ", Column: " << column_;

        if (!error_.empty())
        {
            oss << ", Error: " << error_;
        }

        return oss.str();
    }

    inline void advance(size_t &i, int &column)
    {
        ++i;
        ++column;
    }

    Token Lexer::GetNextToken()
    {
        while (index_ < input_.length())
        {
            char currentChar = input_[index_];

            if (std::isspace(currentChar))
            {
                handleWhitespace();
                continue;
            }

            if (currentChar == ';')
            { // Assuming comments start with a semicolon
                handleComment();
                continue;
            }

            if (currentChar == '(')
                return makeToken(TokenType::OPEN_PAREN);
            if (currentChar == ')')
                return makeToken(TokenType::CLOSE_PAREN);
            if (std::isdigit(currentChar) || currentChar == '+' || currentChar == '-')
            {
                return handleNumber();
            }
            if (std::isalpha(currentChar) || currentChar == '_')
            {
                return handleAlpha();
            }
            if (currentChar == '"')
                return tokenizeString();

            return makeErrorToken("Unknown character");
        }

        return makeEOFToken();
    }

    void Lexer::handleComment()
    {
        while (index_ < input_.length() && input_[index_] != '\n')
        {
            advance(index_, column_);
        }
        if (index_ < input_.length() && input_[index_] == '\n')
        {
            handleWhitespace(); // Handle newline to update line number
        }
    }

    void Lexer::handleWhitespace()
    {
        if (input_[index_] == '\n')
        {
            line_++;
            column_ = 1;
        }
        else
        {
            column_++;
        }
        index_++;
    }

    Token Lexer::makeToken(TokenType type)
    {
        Token token{type, std::string_view(&input_[index_], 1), line_, column_, ""};
        advance(index_, column_);
        return token;
    }

    Token Lexer::makeErrorToken(const std::string &errorMessage)
    {
        std::string_view errorValue(&input_[index_], 1);
        Token errorToken{TokenType::ERROR, errorValue, line_, column_, errorMessage};
        advance(index_, column_);
        return errorToken;
    }

    Token Lexer::makeEOFToken()
    {
        return Token{TokenType::END_OF_FILE, std::string_view(nullptr, 0), line_, column_, ""};
    }

    Token Lexer::handleNumber()
    {
        size_t start = index_;
        bool hasDecimal = false;
        bool hasExponent = false;

        if (input_[index_] == '+' || input_[index_] == '-')
        {
            advance(index_, column_);
        }

        while (index_ < input_.length())
        {
            char c = input_[index_];
            if (c == '.' && !hasDecimal)
            {
                hasDecimal = true;
                advance(index_, column_);
            }
            else if ((c == 'e' || c == 'E') && !hasExponent)
            {
                hasExponent = true;
                advance(index_, column_);

                if (index_ < input_.length() && (input_[index_] == '+' || input_[index_] == '-'))
                {
                    advance(index_, column_);
                }
                if (index_ >= input_.length() || !std::isdigit(input_[index_]))
                {
                    return makeErrorToken("Invalid scientific notation");
                }
            }
            else if (!std::isdigit(c))
            {
                break;
            }
            else
            {
                advance(index_, column_);
            }
        }

        if (hasDecimal || hasExponent)
        {
            return tokenizeFloat(start);
        }
        else
        {
            return tokenizeInteger(start);
        }
    }

    Token Lexer::handleAlpha()
    {
        size_t start = index_;
        while (index_ < input_.length() && (std::isalnum(input_[index_]) || input_[index_] == '_' || input_[index_] == '-' || input_[index_] == '?'))
        {
            advance(index_, column_);
        }

        std::string_view value(input_.data() + start, index_ - start);

        if (value == "true")
        {
            return Token{TokenType::BOOL_TRUE, value, line_, column_, ""};
        }
        else if (value == "false")
        {
            return Token{TokenType::BOOL_FALSE, value, line_, column_, ""};
        }
        else
        {
            return Token{TokenType::SYMBOL, value, line_, column_, ""};
        }
    }

    Token Lexer::tokenizeFloat(size_t start)
    {
        std::string_view value(input_.data() + start, index_ - start);
        return Token{TokenType::FLOAT, value, line_, column_, ""};
    }

    Token Lexer::tokenizeInteger(size_t start)
    {
        std::string_view value(input_.data() + start, index_ - start);
        return Token{TokenType::INTEGER, value, line_, column_, ""};
    }

    Token Lexer::tokenizeString()
    {
        size_t start = index_;
        advance(index_, column_); // Skip the opening quote

        while (index_ < input_.length() && input_[index_] != '"')
        {
            if (input_[index_] == '\n')
            {
                line_++;
                column_ = 0;
            }
            else
            {
                advance(index_, column_);
            }
        }

        if (index_ < input_.length())
        {
            advance(index_, column_); // Include closing quote
            std::string_view value(input_.data() + start, index_ - start);
            return Token{TokenType::STRING, value, line_, column_, ""};
        }
        else
        {
            return Token{TokenType::ERROR, "", line_, column_, "Unterminated string"};
        }
    }

    std::vector<Token> Tokenize(const std::string &input)
    {
        std::vector<Token> tokens;
        Lexer lexer(input);
        Token token;

        do
        {
            token = lexer.GetNextToken();
            tokens.push_back(token);
        } while (token.type_ != TokenType::END_OF_FILE && token.type_ != TokenType::ERROR);

        return tokens;
    }
}