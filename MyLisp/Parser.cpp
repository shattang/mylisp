#include <Shattang/MyLisp/Parser.h>

#include <sstream>

namespace Shattang::MyLisp
{

    // Converts a NodeType to its string representation
    std::string ASTNodeTypeToString(NodeType type)
    {
        switch (type)
        {
        case NodeType::SYMBOL:
            return "SYMBOL";
        case NodeType::INTEGER:
            return "INTEGER";
        case NodeType::FLOAT:
            return "FLOAT";
        case NodeType::BOOLEAN:
            return "BOOLEAN";
        case NodeType::STRING:
            return "STRING";
        case NodeType::VARIABLE_DECLARATION:
            return "VARIABLE_DECLARATION";
        case NodeType::FUNCTION_DECLARATION:
            return "FUNCTION_DECLARATION";
        case NodeType::FUNCTION_CALL:
            return "FUNCTION_CALL";
        default:
            return "UNKNOWN";
        }
    }

    // SymbolNode implementation
    SymbolNode::SymbolNode(const std::string &name) : name_(name) {}

    NodeType SymbolNode::getType() const
    {
        return NodeType::SYMBOL;
    }

    std::string SymbolNode::toString() const
    {
        return "Symbol: " + name_;
    }

    // IntegerNode implementation
    IntegerNode::IntegerNode(long value) : value_(value) {}

    NodeType IntegerNode::getType() const
    {
        return NodeType::INTEGER;
    }

    std::string IntegerNode::toString() const
    {
        return "Integer: " + std::to_string(value_);
    }

    // FloatNode implementation
    FloatNode::FloatNode(double value) : value_(value) {}

    NodeType FloatNode::getType() const
    {
        return NodeType::FLOAT;
    }

    std::string FloatNode::toString() const
    {
        return "Float: " + std::to_string(value_);
    }

    // BooleanNode implementation
    BooleanNode::BooleanNode(bool value) : value(value) {}

    NodeType BooleanNode::getType() const
    {
        return NodeType::BOOLEAN;
    }

    std::string BooleanNode::toString() const
    {
        return "Boolean: " + std::string(value ? "true" : "false");
    }

    // StringNode implementation
    StringNode::StringNode(const std::string &value) : value(value) {}

    NodeType StringNode::getType() const
    {
        return NodeType::STRING;
    }

    std::string StringNode::toString() const
    {
        return "String: " + std::string(value);
    }

    // VariableDeclarationNode implementation
    VariableDeclarationNode::VariableDeclarationNode(const std::string &variableName,
                                                     std::unique_ptr<SymbolNode> typeNode,
                                                     std::unique_ptr<ASTNode> valueNode)
        : variableName(variableName), typeNode(std::move(typeNode)), valueNode(std::move(valueNode)) {}

    NodeType VariableDeclarationNode::getType() const
    {
        return NodeType::VARIABLE_DECLARATION;
    }

    std::string VariableDeclarationNode::toString() const
    {
        return "VariableDeclaration: " + variableName +
               " of type " + typeNode->toString() +
               " = " + valueNode->toString();
    }

    // FunctionDeclarationNode implementation
    FunctionDeclarationNode::FunctionDeclarationNode(const std::string &functionName,
                                                     std::vector<Parameter> parameters,
                                                     std::unique_ptr<SymbolNode> returnType,
                                                     std::vector<std::unique_ptr<ASTNode>> body)
        : functionName(functionName),
          parameters(std::move(parameters)),
          returnType(std::move(returnType)),
          body(std::move(body)) {}

    NodeType FunctionDeclarationNode::getType() const
    {
        return NodeType::FUNCTION_DECLARATION;
    }

    std::string FunctionDeclarationNode::toString() const
    {
        std::ostringstream oss;
        oss << "FunctionDeclaration: " << functionName << "(";
        for (const auto &param : parameters)
        {
            oss << param.name << ": " << param.type->toString() << ", ";
        }
        oss << ") -> " << returnType->toString()
            << " { ";
        for (const auto &statement : body)
        {
            oss << "\t" << statement->toString() << "; ";
        }
        oss << "}";
        return oss.str();
    }

    // FunctionCallNode implementation
    FunctionCallNode::FunctionCallNode(const std::string &functionName,
                                       std::vector<std::unique_ptr<ASTNode>> arguments)
        : functionName(functionName), arguments(std::move(arguments)) {}

    NodeType FunctionCallNode::getType() const
    {
        return NodeType::FUNCTION_CALL;
    }

    std::string FunctionCallNode::toString() const
    {
        std::ostringstream oss;
        oss << "FunctionCall: " << functionName << "(";
        for (const auto &arg : arguments)
        {
            oss << arg->toString() << ", ";
        }
        oss << ")";
        return oss.str();
    }

    ScriptNode::ScriptNode(std::vector<std::unique_ptr<ASTNode>> statements) : statements_(std::move(statements))
    {
    }

    NodeType ScriptNode::getType() const
    {
        return NodeType::SCRIPT;
    }

    std::string ScriptNode::toString() const
    {
        std::ostringstream oss;
        oss << "Script: " << "(";
        for (const auto &arg : statements_)
        {
            oss << "\t" << arg->toString() << "\n";
        }
        oss << ")";
        return oss.str();
    }

    // Parser constructor
    Parser::Parser(Lexer &lexer) : lexer(lexer), currentToken(lexer.GetNextToken()) {}

    // Main parsing function
    std::unique_ptr<ASTNode> Parser::parse()
    {
        std::vector<std::unique_ptr<ASTNode>> statements;
        while(currentToken.type != TokenType::END_OF_FILE)
        {
            statements.push_back(parseExpression());
        }
        return std::make_unique<ScriptNode>(std::move(statements));
    }

    // Parsing expressions
    std::unique_ptr<ASTNode> Parser::parseExpression()
    {
        // Check if the expression starts with an opening parenthesis
        if (currentToken.type == TokenType::OPEN_PAREN)
        {
            consume(TokenType::OPEN_PAREN);

            if (currentToken.type == TokenType::SYMBOL)
            {
                if (currentToken.value == "let")
                {
                    auto letNode = parseLet();
                    consume(TokenType::CLOSE_PAREN); // Consume closing parenthesis for let expression
                    return letNode;
                }
                else if (currentToken.value == "define")
                {
                    auto defineNode = parseDefine();
                    consume(TokenType::CLOSE_PAREN); // Consume closing parenthesis for define expression
                    return defineNode;
                }
                else
                {
                    auto functionCallNode = parseFunctionCall();
                    consume(TokenType::CLOSE_PAREN); // Consume closing parenthesis for function call
                    return functionCallNode;
                }
            }
            else
            {
                auto atomNode = parseAtom();
                consume(TokenType::CLOSE_PAREN); // Consume closing parenthesis for single atom expression
                return atomNode;
            }
        }

        // If not an open parenthesis, assume it's a standalone atom
        return parseAtom();
    }

    // Parsing atomic expressions
    std::unique_ptr<ASTNode> Parser::parseAtom()
    {
        auto doParse = [this](TokenType type, std::unique_ptr<ASTNode>&& node)
        {
            consume(type);
            return std::move(node);
        };

        switch (currentToken.type)
        {
        case TokenType::SYMBOL:
            return doParse(currentToken.type, std::make_unique<SymbolNode>(std::string(currentToken.value)));
        case TokenType::INTEGER:
            return doParse(currentToken.type, std::make_unique<IntegerNode>(std::stol(std::string(currentToken.value))));
        case TokenType::FLOAT:
            return doParse(currentToken.type, std::make_unique<FloatNode>(std::stod(std::string(currentToken.value))));
        case TokenType::BOOL_TRUE:
            return doParse(currentToken.type, std::make_unique<BooleanNode>(true));
        case TokenType::BOOL_FALSE:
            return doParse(currentToken.type, std::make_unique<BooleanNode>(false));
        case TokenType::STRING:
            return doParse(currentToken.type, std::make_unique<StringNode>(std::string(currentToken.value)));
        default:
            throwError("Unexpected token: " + TokenTypeToString(currentToken.type));
        }
        return nullptr;
    }

    // Parsing let expressions
    std::unique_ptr<ASTNode> Parser::parseLet()
    {
        consume(TokenType::SYMBOL); // Consume `let`

        consume(TokenType::OPEN_PAREN); // Consume opening parenthesis for variable declaration
        if (currentToken.type != TokenType::SYMBOL)
        {
            throwError("Expected a variable name after 'let'");
        }
        std::string varName = std::string(currentToken.value);
        consume(TokenType::SYMBOL);

        if (currentToken.type != TokenType::SYMBOL)
        {
            throwError("Expected a type after variable name");
        }
        std::unique_ptr<SymbolNode> typeNode = std::make_unique<SymbolNode>(std::string(currentToken.value));
        consume(TokenType::SYMBOL);
        consume(TokenType::CLOSE_PAREN); // Consume closing parenthesis for variable declaration

        std::unique_ptr<ASTNode> valueNode = parseExpression();

        return std::make_unique<VariableDeclarationNode>(varName, std::move(typeNode), std::move(valueNode));
    }

    // Parsing function definitions
    std::unique_ptr<ASTNode> Parser::parseDefine()
    {
        consume(TokenType::SYMBOL); // Consume `define`

        if (currentToken.type != TokenType::SYMBOL)
        {
            throwError("Expected a function name after 'define'");
        }
        std::string funcName = std::string(currentToken.value);
        consume(TokenType::SYMBOL);

        std::vector<Parameter> parameters;
        consume(TokenType::OPEN_PAREN); // Consume the opening parenthesis for parameter list
        while (currentToken.type != TokenType::CLOSE_PAREN)
        {
            consume(TokenType::OPEN_PAREN); // Consume the opening parenthesis for each parameter
            if (currentToken.type != TokenType::SYMBOL)
            {
                throwError("Expected a parameter name");
            }
            std::string paramName = std::string(currentToken.value);
            consume(TokenType::SYMBOL);

            if (currentToken.type != TokenType::SYMBOL)
            {
                throwError("Expected a parameter type");
            }
            auto paramType = std::make_unique<SymbolNode>(std::string(currentToken.value));
            consume(TokenType::SYMBOL);
            consume(TokenType::CLOSE_PAREN); // Consume the closing parenthesis for each parameter

            parameters.emplace_back(Parameter{paramName, std::move(paramType)});
        }
        consume(TokenType::CLOSE_PAREN); // Consume the closing parenthesis for the parameter list

        if (currentToken.type != TokenType::SYMBOL)
        {
            throwError("Expected a return type for the function");
        }
        auto returnType = std::make_unique<SymbolNode>(std::string(currentToken.value));
        consume(TokenType::SYMBOL);

        std::vector<std::unique_ptr<ASTNode>> body;
        while (currentToken.type != TokenType::CLOSE_PAREN)
        {
            body.push_back(parseExpression());
        }
        
        return std::make_unique<FunctionDeclarationNode>(funcName, std::move(parameters), std::move(returnType), std::move(body));
    }

    // Parsing function calls
    std::unique_ptr<ASTNode> Parser::parseFunctionCall()
    {
        if (currentToken.type != TokenType::SYMBOL)
        {
            throwError("Expected a function name");
        }
        std::string funcName = std::string(currentToken.value);
        consume(TokenType::SYMBOL);

        std::vector<std::unique_ptr<ASTNode>> arguments;
        while (currentToken.type != TokenType::CLOSE_PAREN)
        {
            arguments.push_back(parseExpression());
        }

        return std::make_unique<FunctionCallNode>(funcName, std::move(arguments));
    }

    // Consuming expected tokens and error handling
    void Parser::consume(TokenType expectedType)
    {
        if (currentToken.type == expectedType)
        {
            currentToken = lexer.GetNextToken();
        }
        else
        {
            std::ostringstream oss;
            oss << "Unexpected token: expected " << TokenTypeToString(expectedType)
                << ", but got " << TokenTypeToString(currentToken.type) << " '" << currentToken.value << "'"
                << " at line " << currentToken.line << ", column " << currentToken.column;
            throw std::runtime_error(oss.str());
        }
    }

    void Parser::throwError(const std::string &message)
    {
        std::ostringstream oss;
        oss << "Parse error: " << message
            << " at line " << currentToken.line
            << ", column " << currentToken.column;
        throw std::runtime_error(oss.str());
    }

    void ASTNode::visit(ASTVisitor &visit)
    {
        visit.visit(*this);
    }

} // namespace Shattang::MyLisp
