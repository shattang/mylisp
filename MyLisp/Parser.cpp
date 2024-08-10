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
        case NodeType::VARIABLE_ASSIGNMENT:
            return "VARIABLE_ASSIGNMENT";
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
    BooleanNode::BooleanNode(bool value) : value_(value) {}

    NodeType BooleanNode::getType() const
    {
        return NodeType::BOOLEAN;
    }

    std::string BooleanNode::toString() const
    {
        return "Boolean: " + std::string(value_ ? "true" : "false");
    }

    // StringNode implementation
    StringNode::StringNode(const std::string &value) : value_(value) {}

    NodeType StringNode::getType() const
    {
        return NodeType::STRING;
    }

    std::string StringNode::toString() const
    {
        return "String: " + std::string(value_);
    }

    // VariableDeclarationNode implementation
    VariableDeclarationNode::VariableDeclarationNode(const std::string &variableName,
                                                     std::unique_ptr<SymbolNode> typeNode,
                                                     std::unique_ptr<ASTNode> valueNode)
        : variableName_(variableName), typeNode_(std::move(typeNode)), valueNode_(std::move(valueNode)) {}

    NodeType VariableDeclarationNode::getType() const
    {
        return NodeType::VARIABLE_DECLARATION;
    }

    std::string VariableDeclarationNode::toString() const
    {
        return "VariableDeclaration: " + variableName_ +
               " of type " + typeNode_->toString() +
               " = " + valueNode_->toString();
    }

    // FunctionDeclarationNode implementation
    FunctionDeclarationNode::FunctionDeclarationNode(const std::string &functionName,
                                                     std::vector<Parameter> parameters,
                                                     std::unique_ptr<SymbolNode> returnType,
                                                     std::vector<std::unique_ptr<ASTNode>> body)
        : functionName_(functionName),
          parameters_(std::move(parameters)),
          returnType_(std::move(returnType)),
          body_(std::move(body)) {}

    NodeType FunctionDeclarationNode::getType() const
    {
        return NodeType::FUNCTION_DECLARATION;
    }

    std::string FunctionDeclarationNode::toString() const
    {
        std::ostringstream oss;
        oss << "FunctionDeclaration: " << functionName_ << "(";
        for (const auto &param : parameters_)
        {
            oss << param.name_ << ": " << param.type_->toString() << ", ";
        }
        oss << ") -> " << returnType_->toString()
            << " { " << "\n";
        for (const auto &statement : body_)
        {
            oss << "\t\t" << statement->toString() << ";\n";
        }
        oss << "\t}";
        return oss.str();
    }

    // FunctionCallNode implementation
    FunctionCallNode::FunctionCallNode(const std::string &functionName,
                                       std::vector<std::unique_ptr<ASTNode>> arguments)
        : functionName_(functionName), arguments_(std::move(arguments)) {}

    NodeType FunctionCallNode::getType() const
    {
        return NodeType::FUNCTION_CALL;
    }

    std::string FunctionCallNode::toString() const
    {
        std::ostringstream oss;
        oss << "FunctionCall: " << functionName_ << "(";
        for (const auto &arg : arguments_)
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

    VariableAssignmentNode::VariableAssignmentNode(const std::string &variableName, std::unique_ptr<ASTNode> valueNode)
        : variableName_(variableName), valueNode_(std::move(valueNode)) {}

    NodeType VariableAssignmentNode::getType() const
    {
        return NodeType::VARIABLE_ASSIGNMENT;
    }

    std::string VariableAssignmentNode::toString() const
    {
        return "VariableAssignment: " + variableName_ + " = " + valueNode_->toString();
    }

    ForIterationNode::ForIterationNode(const std::string &index,
                                       std::unique_ptr<ASTNode> start,
                                       std::unique_ptr<ASTNode> end,
                                       std::unique_ptr<ASTNode> step,
                                       std::vector<std::unique_ptr<ASTNode>> body)
        : index_(index), start_(std::move(start)), end_(std::move(end)), step_(std::move(step)), body_(std::move(body)) {}

    NodeType ForIterationNode::getType() const
    {
        return NodeType::FOR_ITERATION;
    }

    std::string ForIterationNode::toString() const
    {
        std::ostringstream oss;
        oss << "ForIteration: " << index_ << " from " << start_->toString() << " to " << end_->toString()
            << " step " << step_->toString() << " { ";
        for (const auto &expr : body_)
        {
            oss << expr->toString() << "; ";
        }
        oss << "}";
        return oss.str();
    }

    WhileIterationNode::WhileIterationNode(std::unique_ptr<ASTNode> condition, std::vector<std::unique_ptr<ASTNode>> body)
        : condition_(std::move(condition)), body_(std::move(body)) {}

    NodeType WhileIterationNode::getType() const
    {
        return NodeType::WHILE_ITERATION;
    }

    std::string WhileIterationNode::toString() const
    {
        std::ostringstream oss;
        oss << "WhileIteration (" << condition_->toString() << ") { ";
        for (const auto &expr : body_)
        {
            oss << expr->toString() << "; ";
        }
        oss << "}";
        return oss.str();
    }

    IfNode::IfNode(std::unique_ptr<ASTNode> condition,
                   std::unique_ptr<ASTNode> thenBranch,
                   std::unique_ptr<ASTNode> elseBranch)
        : condition_(std::move(condition)),
          thenBranch_(std::move(thenBranch)),
          elseBranch_(std::move(elseBranch)) {}

    NodeType IfNode::getType() const
    {
        return NodeType::IF;
    }

    std::string IfNode::toString() const
    {
        std::ostringstream oss;
        oss << "If (" << condition_->toString() << ") "
            << "Then {" << thenBranch_->toString() << "} "
            << "Else {" << elseBranch_->toString() << "}";
        return oss.str();
    }

    // Parser constructor
    Parser::Parser(Lexer &lexer) : lexer_(lexer), currentToken_(lexer.GetNextToken()) {}

    // Main parsing function
    std::unique_ptr<ASTNode> Parser::parse()
    {
        std::vector<std::unique_ptr<ASTNode>> statements;
        while (currentToken_.type_ != TokenType::END_OF_FILE)
        {
            statements.push_back(parseExpression());
        }
        return std::make_unique<ScriptNode>(std::move(statements));
    }

    // Parsing expressions
    std::unique_ptr<ASTNode> Parser::parseExpression()
    {
        int openParenCount = 0;

        // Unwrap nested parentheses
        while (currentToken_.type_ == TokenType::OPEN_PAREN)
        {
            consume(TokenType::OPEN_PAREN);
            openParenCount++;
        }

        // Parse the actual expression after unwrapping
        std::unique_ptr<ASTNode> expr;
        if (currentToken_.type_ == TokenType::SYMBOL)
        {
            if (currentToken_.value_ == "let")
            {
                expr = parseLet();
            }
            else if (currentToken_.value_ == "define")
            {
                expr = parseDefine();
            }
            else if (currentToken_.value_ == "set")
            {
                expr = parseSet();
            }
            else if (currentToken_.value_ == "for")
            {
                expr = parseForIteration();
            }
            else if (currentToken_.value_ == "while")
            {
                expr = parseWhileIteration();
            }
            else if (currentToken_.value_ == "if")
            {
                expr = parseIf();
            }
            else if (openParenCount > 0)
            {
                expr = parseFunctionCall();
            }
            else
            {
                expr = parseAtom();
            }
        }
        else
        {
            expr = parseAtom();
        }

        // Consume the matching number of closing parentheses
        while (openParenCount > 0 && currentToken_.type_ == TokenType::CLOSE_PAREN)
        {
            consume(TokenType::CLOSE_PAREN);
            openParenCount--;
        }

        // If there are still unclosed opening parentheses, it should be an error
        if (openParenCount != 0)
        {
            throwError("Mismatched parentheses: more opening than closing parentheses.");
        }

        return expr;
    }

    std::unique_ptr<ASTNode> Parser::parseIf()
    {
        consume(TokenType::SYMBOL); // Consume `if`

        auto condition = parseExpression();  // Parse the condition
        auto thenBranch = parseExpression(); // Parse the then-branch
        auto elseBranch = parseExpression(); // Parse the else-branch

        return std::make_unique<IfNode>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
    }

    std::unique_ptr<ASTNode> Parser::parseForIteration()
    {
        consume(TokenType::SYMBOL); // Consume `for`

        if (currentToken_.type_ != TokenType::SYMBOL)
        {
            throwError("Expected an index variable name after 'for'");
        }
        std::string index = std::string(currentToken_.value_);
        consume(TokenType::SYMBOL);

        auto start_ = parseExpression(); // Parse the start expression
        auto end_ = parseExpression();   // Parse the end expression
        auto step_ = parseExpression();  // Parse the step expression

        std::vector<std::unique_ptr<ASTNode>> body_;
        while (currentToken_.type_ != TokenType::CLOSE_PAREN)
        {
            body_.push_back(parseExpression()); // Parse each expression in the loop body
        }

        return std::make_unique<ForIterationNode>(index, std::move(start_), std::move(end_), std::move(step_), std::move(body_));
    }

    std::unique_ptr<ASTNode> Parser::parseWhileIteration()
    {
        consume(TokenType::SYMBOL); // Consume `while`

        auto condition = parseExpression(); // Parse the condition

        std::vector<std::unique_ptr<ASTNode>> body;
        while (currentToken_.type_ != TokenType::CLOSE_PAREN)
        {
            body.push_back(parseExpression()); // Parse each expression in the loop body
        }

        return std::make_unique<WhileIterationNode>(std::move(condition), std::move(body));
    }

    std::unique_ptr<ASTNode> Parser::parseSet()
    {
        consume(TokenType::SYMBOL); // Consume `set`

        if (currentToken_.type_ != TokenType::SYMBOL)
        {
            throwError("Expected a variable name after 'set'");
        }
        std::string variableName = std::string(currentToken_.value_);
        consume(TokenType::SYMBOL);

        std::unique_ptr<ASTNode> valueNode = parseExpression(); // Parse the new value

        return std::make_unique<VariableAssignmentNode>(variableName, std::move(valueNode));
    }

    // Parsing atomic expressions
    std::unique_ptr<ASTNode> Parser::parseAtom()
    {
        auto doParse = [this](TokenType type, std::unique_ptr<ASTNode> &&node)
        {
            consume(type);
            return std::move(node);
        };

        switch (currentToken_.type_)
        {
        case TokenType::SYMBOL:
            return doParse(currentToken_.type_, std::make_unique<SymbolNode>(std::string(currentToken_.value_)));
        case TokenType::INTEGER:
            return doParse(currentToken_.type_, std::make_unique<IntegerNode>(std::stol(std::string(currentToken_.value_))));
        case TokenType::FLOAT:
            return doParse(currentToken_.type_, std::make_unique<FloatNode>(std::stod(std::string(currentToken_.value_))));
        case TokenType::BOOL_TRUE:
            return doParse(currentToken_.type_, std::make_unique<BooleanNode>(true));
        case TokenType::BOOL_FALSE:
            return doParse(currentToken_.type_, std::make_unique<BooleanNode>(false));
        case TokenType::STRING:
            return doParse(currentToken_.type_, std::make_unique<StringNode>(std::string(currentToken_.value_)));
        default:
            throwError("Unexpected token: " + TokenTypeToString(currentToken_.type_));
        }
        return nullptr;
    }

    // Parsing let expressions
    std::unique_ptr<ASTNode> Parser::parseLet()
    {
        consume(TokenType::SYMBOL); // Consume `let`

        consume(TokenType::OPEN_PAREN); // Consume opening parenthesis for variable declaration
        if (currentToken_.type_ != TokenType::SYMBOL)
        {
            throwError("Expected a variable name after 'let'");
        }
        std::string varName = std::string(currentToken_.value_);
        consume(TokenType::SYMBOL);

        if (currentToken_.type_ != TokenType::SYMBOL)
        {
            throwError("Expected a type after variable name");
        }
        std::unique_ptr<SymbolNode> typeNode = std::make_unique<SymbolNode>(std::string(currentToken_.value_));
        consume(TokenType::SYMBOL);
        consume(TokenType::CLOSE_PAREN); // Consume closing parenthesis for variable declaration

        std::unique_ptr<ASTNode> valueNode = parseExpression();

        return std::make_unique<VariableDeclarationNode>(varName, std::move(typeNode), std::move(valueNode));
    }

    // Parsing function definitions
    std::unique_ptr<ASTNode> Parser::parseDefine()
    {
        if (isParsingDefine_)
            throwError("Nested function definition not supported");

        isParsingDefine_ = true;

        consume(TokenType::SYMBOL); // Consume `define`

        if (currentToken_.type_ != TokenType::SYMBOL)
        {
            throwError("Expected a function name after 'define'");
        }
        std::string funcName = std::string(currentToken_.value_);
        consume(TokenType::SYMBOL);

        std::vector<Parameter> parameters;
        consume(TokenType::OPEN_PAREN); // Consume the opening parenthesis for parameter list
        while (currentToken_.type_ != TokenType::CLOSE_PAREN)
        {
            consume(TokenType::OPEN_PAREN); // Consume the opening parenthesis for each parameter
            if (currentToken_.type_ != TokenType::SYMBOL)
            {
                throwError("Expected a parameter name");
            }
            std::string paramName = std::string(currentToken_.value_);
            consume(TokenType::SYMBOL);

            if (currentToken_.type_ != TokenType::SYMBOL)
            {
                throwError("Expected a parameter type");
            }
            auto paramType = std::make_unique<SymbolNode>(std::string(currentToken_.value_));
            consume(TokenType::SYMBOL);
            consume(TokenType::CLOSE_PAREN); // Consume the closing parenthesis for each parameter

            parameters.emplace_back(Parameter{paramName, std::move(paramType)});
        }
        consume(TokenType::CLOSE_PAREN); // Consume the closing parenthesis for the parameter list

        if (currentToken_.type_ != TokenType::SYMBOL)
        {
            throwError("Expected a return type for the function");
        }
        auto returnType = std::make_unique<SymbolNode>(std::string(currentToken_.value_));
        consume(TokenType::SYMBOL);

        std::vector<std::unique_ptr<ASTNode>> body;
        while (currentToken_.type_ != TokenType::CLOSE_PAREN)
        {
            body.push_back(parseExpression());
        }

        isParsingDefine_ = false;
        return std::make_unique<FunctionDeclarationNode>(funcName, std::move(parameters), std::move(returnType), std::move(body));
    }

    // Parsing function calls
    std::unique_ptr<ASTNode> Parser::parseFunctionCall()
    {
        if (currentToken_.type_ != TokenType::SYMBOL)
        {
            throwError("Expected a function name");
        }
        std::string funcName = std::string(currentToken_.value_);
        consume(TokenType::SYMBOL);

        std::vector<std::unique_ptr<ASTNode>> arguments;
        while (currentToken_.type_ != TokenType::CLOSE_PAREN)
        {
            arguments.push_back(parseExpression());
        }

        return std::make_unique<FunctionCallNode>(funcName, std::move(arguments));
    }

    // Consuming expected tokens and error handling
    void Parser::consume(TokenType expectedType)
    {
        if (currentToken_.type_ == expectedType)
        {
            currentToken_ = lexer_.GetNextToken();
        }
        else
        {
            std::ostringstream oss;
            oss << "Unexpected token: expected " << TokenTypeToString(expectedType)
                << ", but got " << TokenTypeToString(currentToken_.type_) << " '" << currentToken_.value_ << "'"
                << " at line " << currentToken_.line_ << ", column " << currentToken_.column_;
            throw std::runtime_error(oss.str());
        }
    }

    void Parser::throwError(const std::string &message)
    {
        std::ostringstream oss;
        oss << "Parse error: " << message
            << " at line " << currentToken_.line_
            << ", column " << currentToken_.column_
            << ": '" << currentToken_.value_ << "'";
        throw std::runtime_error(oss.str());
    }

    void ASTNode::visit(ASTVisitor &visit) const
    {
        visit.visit(*this);
    }

} // namespace Shattang::MyLisp
