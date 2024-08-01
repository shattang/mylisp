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

    void tokenizeNumber(const std::string &input, size_t &i, int line, int& column, std::vector<Token> &tokens)
    {
        std::string value(1, input[i]);
        bool hasDecimal = (input[i] == '.');
        int startColumn = column; // Store the starting column of the number

        while (i + 1 < input.length() && (std::isdigit(input[i + 1]) || input[i + 1] == '.'))
        {
            // Check for multiple decimal points
            if (input[i + 1] == '.' && hasDecimal)
            {
                value += input[++i];

                // Consume the rest of the invalid number
                while (i + 1 < input.length() && (std::isdigit(input[i + 1]) || input[i + 1] == '.'))
                {
                    value += input[++i];
                }

                // Create the ERROR token with a concatenated error message
                std::string errorMessage = "Invalid number: multiple decimal points in '" + value + "'";
                tokens.push_back({TokenType::ERROR, errorMessage, line, startColumn}); // Use startColumn
                return;                                                                // Exit the function early on error
            }
            if (input[i + 1] == '.')
            {
                hasDecimal = true;
            }
            value += input[++i];
            ++column; // Increment column for each character added to value
        }

        // Check for trailing decimal
        if (value.back() == '.')
        {
            std::string errorMessage = "Invalid number: trailing decimal in '" + value + "'";
            tokens.push_back({TokenType::ERROR, errorMessage, line, startColumn}); // Use startColumn
        }
        else
        {
            tokens.push_back({TokenType::NUMBER, value, line, startColumn}); // Use startColumn
        }
    }

    // Function to tokenize a string
    void tokenizeString(const std::string &input, size_t &i, int line, int column, std::vector<Token> &tokens)
    {
        std::string value(1, input[i]); // Include the opening quote
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
            tokens.push_back({TokenType::ERROR, "Unterminated string", line, column});
        }
    }

    // Function to tokenize a symbol
    void tokenizeSymbol(const std::string &input, size_t &i, int line, int column, std::vector<Token> &tokens)
    {
        std::string value(1, input[i]);
        while (i + 1 < input.length() && (std::isalnum(input[i + 1]) || input[i + 1] == '-' || input[i + 1] == '_'))
        {
            value += input[++i];
        }
        tokens.push_back({TokenType::SYMBOL, value, line, column});
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
            if (c == ';')
            {
                while (i < input.length() && input[i] != '\n')
                    ++i;
                continue;
            }

            switch (c)
            {
            case '(':
                tokens.push_back({TokenType::OPEN_PAREN, "(", line, column});
                break;
            case ')':
                tokens.push_back({TokenType::CLOSE_PAREN, ")", line, column});
                break;
            case '\'':
                tokens.push_back({TokenType::QUOTE, "'", line, column});
                break;
            case '+':
            case '-':
                if (i + 1 < input.length() && (std::isdigit(input[i + 1]) || input[i + 1] == '.'))
                {
                    tokenizeNumber(input, i, line, column, tokens);
                }
                else
                {
                    tokens.push_back({(c == '+' ? TokenType::PLUS : TokenType::MINUS), std::string(1, c), line, column});
                }
                break;
            case '*':
                tokens.push_back({TokenType::STAR, "*", line, column});
                break;
            case '/':
                tokens.push_back({TokenType::SLASH, "/", line, column});
                break;
            case '%':
                tokens.push_back({TokenType::PERCENT, "%", line, column});
                break;
            case '<':
                if (i + 1 < input.length() && input[i + 1] == '=')
                {
                    tokens.push_back({TokenType::LEQ, "<=", line, column});
                    ++i;
                }
                else
                {
                    tokens.push_back({TokenType::LT, "<", line, column});
                }
                break;
            case '>':
                if (i + 1 < input.length() && input[i + 1] == '=')
                {
                    tokens.push_back({TokenType::GEQ, ">=", line, column});
                    ++i;
                }
                else
                {
                    tokens.push_back({TokenType::GT, ">", line, column});
                }
                break;
            case '=':
                if (i + 1 < input.length() && input[i + 1] == '=')
                {
                    tokens.push_back({TokenType::EQ, "==", line, column});
                    ++i;
                }
                else
                {
                    tokens.push_back({TokenType::ERROR, std::string("Invalid character: ") + c, line, column});
                }
                break;
            case '!':
                if (i + 1 < input.length() && input[i + 1] == '=')
                {
                    tokens.push_back({TokenType::NEQ, "!=", line, column});
                    ++i;
                }
                else
                {
                    tokens.push_back({TokenType::ERROR, std::string("Invalid character: ") + c, line, column});
                }
                break;
            case '.':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                tokenizeNumber(input, i, line, column, tokens);
                break;
            case '"':
                tokenizeString(input, i, line, column, tokens);
                break;
            default:
                if (std::isalpha(c))
                {
                    tokenizeSymbol(input, i, line, column, tokens);
                }
                else
                {
                    tokens.push_back({TokenType::ERROR, std::string("Invalid character: ") + c, line, column});
                }
                break;
            }
            ++column;
        }
        return tokens;
    }
}