#pragma once

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>
#include "Lexer.h"

namespace Shattang::MyLisp
{

    enum class NodeType
    {
        SYMBOL,
        INTEGER,
        FLOAT,
        BOOLEAN,
        STRING,
        VARIABLE_DECLARATION,
        FUNCTION_DECLARATION,
        FUNCTION_CALL,
        SCRIPT
    };

    class ASTNode;

    class ASTVisitor
    {
    public:
        virtual ~ASTVisitor() = default;

        virtual void visit(const ASTNode &node) = 0;
    };

    // Base class for AST nodes
    class ASTNode
    {
    public:
        virtual ~ASTNode() = default;
        virtual NodeType getType() const = 0;
        virtual std::string toString() const = 0;
        virtual void visit(ASTVisitor &visit);
    };

    class ScriptNode : public ASTNode
    {
    public:
        std::vector<std::unique_ptr<ASTNode>> statements_;
        ScriptNode(std::vector<std::unique_ptr<ASTNode>> statements);
        NodeType getType() const override;
        std::string toString() const override;
    };

    // Derived class for Symbol Nodes
    class SymbolNode : public ASTNode
    {
    public:
        std::string name_;
        SymbolNode(const std::string &name);
        NodeType getType() const override;
        std::string toString() const override;
    };

    // Base class for Literal Nodes
    class LiteralNode : public ASTNode
    {
    };

    // Derived classes for specific literal types

    class IntegerNode : public LiteralNode
    {
    public:
        long value_;
        IntegerNode(long value);
        NodeType getType() const override;
        std::string toString() const override;
    };

    class FloatNode : public LiteralNode
    {
    public:
        double value_;
        FloatNode(double value);
        NodeType getType() const override;
        std::string toString() const override;
    };

    class BooleanNode : public LiteralNode
    {
    public:
        bool value;
        BooleanNode(bool value);
        NodeType getType() const override;
        std::string toString() const override;
    };

    class StringNode : public LiteralNode
    {
    public:
        std::string value;
        StringNode(const std::string &value);
        NodeType getType() const override;
        std::string toString() const override;
    };

    // Parameter struct used in function declarations
    struct Parameter
    {
        std::string name;
        std::unique_ptr<SymbolNode> type;
    };

    // Derived class for Variable Declaration Nodes
    class VariableDeclarationNode : public ASTNode
    {
    public:
        std::string variableName;
        std::unique_ptr<SymbolNode> typeNode;
        std::unique_ptr<ASTNode> valueNode;
        VariableDeclarationNode(const std::string &variableName,
                                std::unique_ptr<SymbolNode> typeNode,
                                std::unique_ptr<ASTNode> valueNode);
        NodeType getType() const override;
        std::string toString() const override;
    };

    // Derived class for Function Declaration Nodes
    class FunctionDeclarationNode : public ASTNode
    {
    public:
        std::string functionName;
        std::vector<Parameter> parameters;
        std::unique_ptr<SymbolNode> returnType;
        std::vector<std::unique_ptr<ASTNode>> body;
        FunctionDeclarationNode(const std::string &functionName,
                                std::vector<Parameter> parameters,
                                std::unique_ptr<SymbolNode> returnType,
                                std::vector<std::unique_ptr<ASTNode>> body);
        NodeType getType() const override;
        std::string toString() const override;
    };

    // Derived class for Function Call Nodes
    class FunctionCallNode : public ASTNode
    {
    public:
        std::string functionName;
        std::vector<std::unique_ptr<ASTNode>> arguments;
        FunctionCallNode(const std::string &functionName, std::vector<std::unique_ptr<ASTNode>> arguments);
        NodeType getType() const override;
        std::string toString() const override;
    };

    // Converts a NodeType to its string representation
    std::string ASTNodeTypeToString(NodeType type);

    // Parser processes tokens from the Lexer to create an AST.
    class Parser
    {
    private:
        Lexer &lexer;
        Token currentToken;

        std::unique_ptr<ASTNode> parseExpression();
        std::unique_ptr<ASTNode> parseAtom();
        std::unique_ptr<ASTNode> parseLet();
        std::unique_ptr<ASTNode> parseDefine();
        std::unique_ptr<ASTNode> parseFunctionCall();
        void consume(TokenType expectedType);
        void throwError(const std::string &message);

    public:
        explicit Parser(Lexer &lexer);
        std::unique_ptr<ASTNode> parse();
    };

} // namespace Shattang::MyLisp
