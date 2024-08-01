#include <Shattang/MyLisp/lexer.h>
#include <cctype>

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
        case TokenType::QUOTE:
            return "QUOTE";
        case TokenType::SYMBOL:
            return "SYMBOL";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::STRING:
            return "STRING";
        case TokenType::ERROR:
            return "ERROR";
        case TokenType::LT:
            return "LT";
        case TokenType::GT:
            return "GT";
        case TokenType::EQ:
            return "EQ";
        case TokenType::LEQ:
            return "LEQ";
        case TokenType::GEQ:
            return "GEQ";
        case TokenType::NEQ:
            return "NEQ";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::STAR:
            return "STAR";
        case TokenType::SLASH:
            return "SLASH";
        case TokenType::PERCENT:
            return "PERCENT";
        // Add cases for other token types
        default:
            return "UNKNOWN";
        }
    }

    std::string Token::ToString() const
    {
        std::string str = "Type: " + TokenTypeToString(type) +
                          ", Value: '" + value + "'" +
                          ", Line: " + std::to_string(line) +
                          ", Column: " + std::to_string(column);
        return str;
    }

    std::vector<Token> Lexer::Tokenize(const std::string &input)
    {
        std::vector<Token> tokens;
        int line = 1, column = 1;
        for (size_t i = 0; i < input.length(); ++i)
        {
            char c = input[i];
            if (std::isspace(c))
            {
                if (c == '\n')
                {
                    ++line;
                    column = 1;
                }
                else
                {
                    ++column;
                }
                continue;
            }
            // Handle comments (skip everything until newline)
            if (c == ';')
            {
                while (i < input.length() && input[i] != '\n')
                    ++i;
                continue;
            }

            // Check for single-character tokens
            if (c == '(')
            {
                tokens.push_back({TokenType::OPEN_PAREN, "(", line, column});
            }
            else if (c == ')')
            {
                tokens.push_back({TokenType::CLOSE_PAREN, ")", line, column});
            }
            else if (c == '\'')
            {
                tokens.push_back({TokenType::QUOTE, "'", line, column});
            }
            else if (c == '<')
            {
                if (i + 1 < input.length() && input[i + 1] == '=')
                { // <=
                    tokens.push_back({TokenType::LEQ, "<=", line, column});
                    ++i; // Skip the '=' character
                }
                else
                { // <
                    tokens.push_back({TokenType::LT, "<", line, column});
                }
            }
            else if (c == '>')
            {
                if (i + 1 < input.length() && input[i + 1] == '=')
                { // >=
                    tokens.push_back({TokenType::GEQ, ">=", line, column});
                    ++i; // Skip the '=' character
                }
                else
                { // >
                    tokens.push_back({TokenType::GT, ">", line, column});
                }
            }
            else if (c == '=')
            {
                if (i + 1 < input.length() && input[i + 1] == '=')
                { // ==
                    tokens.push_back({TokenType::EQ, "==", line, column});
                    ++i; // Skip the '=' character
                }
                else
                {
                    // Handle invalid '=' character with an error token
                    tokens.push_back({TokenType::ERROR, std::string("Invalid character: ") + c, line, column});
                }
            }
            else if (c == '!')
            {
                if (i + 1 < input.length() && input[i + 1] == '=')
                { // !=
                    tokens.push_back({TokenType::NEQ, "!=", line, column});
                    ++i; // Skip the '=' character
                }
                else
                {
                    // Handle invalid '!' character with an error token
                    tokens.push_back({TokenType::ERROR, std::string("Invalid character: ") + c, line, column});
                }
            }
            else if (c == '+' || c == '-' || std::isdigit(c))
            {
                if (i + 1 < input.length() && std::isdigit(input[i + 1]))
                {
                    std::string value(1, c);
                    size_t numDecimals = 0;
                    while (i + 1 < input.length())
                    {
                        if (std::isdigit(input[i + 1]))
                        {
                            value += input[++i];
                        }
                        else if (input[i + 1] == '.')
                        {
                            if (numDecimals == 0)
                            {
                                numDecimals += 1;
                                value += input[++i];
                            }
                            else
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }

                    tokens.push_back({TokenType::NUMBER, value, line, column});
                }
                else
                {
                    if (c == '+')
                        tokens.push_back({TokenType::PLUS, std::string(1, c), line, column});
                    else if (c == '-')
                        tokens.push_back({TokenType::MINUS, std::string(1, c), line, column});
                    else
                        tokens.push_back({TokenType::NUMBER, std::string(1, c), line, column});
                }
            }
            else if (c == '*')
            {
                tokens.push_back({TokenType::STAR, "*", line, column});
            }
            else if (c == '/')
            {
                tokens.push_back({TokenType::SLASH, "/", line, column});
            }
            else if (c == '%')
            {
                tokens.push_back({TokenType::PERCENT, "%", line, column});
            }
            // ... (add other single-character tokens)
            else
            {
                // Handle multi-character tokens
                std::string value(1, c);
                if (c == '"')
                { // Strings
                    while (i + 1 < input.length() && input[i + 1] != '"')
                    {
                        value += input[++i];
                    }
                    if (i + 1 < input.length())
                    {
                        value += input[++i]; // Include closing quote
                        tokens.push_back({TokenType::STRING, value, line, column});
                    }
                    else
                    {
                        // Handle unterminated strings with an error token
                        tokens.push_back({TokenType::ERROR, "Unterminated string", line, column});
                    }
                }
                else if (std::isalpha(c))
                { // Symbols
                    while (i + 1 < input.length() && (std::isalnum(input[i + 1]) || input[i + 1] == '-' || input[i + 1] == '_'))
                    {
                        value += input[++i];
                    }
                    tokens.push_back({TokenType::SYMBOL, value, line, column});
                }
                else
                {
                    // Handle invalid characters with an error token
                    tokens.push_back({TokenType::ERROR, std::string("Invalid character: ") + c, line, column});
                }
            }
            ++column; // Move to the next column
        }

        return tokens;
    }
}