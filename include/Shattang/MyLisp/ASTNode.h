#pragma once

#include <vector>
#include <string>
#include <memory>

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
        VARIABLE_ASSIGNMENT,
        FOR_ITERATION,
        WHILE_ITERATION,
        IF,
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
        virtual void visit(ASTVisitor &visit) const;
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
        bool value_;
        BooleanNode(bool value);
        NodeType getType() const override;
        std::string toString() const override;
    };

    class StringNode : public LiteralNode
    {
    public:
        std::string value_;
        StringNode(const std::string &value);
        NodeType getType() const override;
        std::string toString() const override;
    };

    // Parameter struct used in function declarations
    struct Parameter
    {
        std::string name_;
        std::unique_ptr<SymbolNode> type_;
    };

    // Derived class for Variable Declaration Nodes
    class VariableDeclarationNode : public ASTNode
    {
    public:
        std::string variableName_;
        std::unique_ptr<SymbolNode> typeNode_;
        std::unique_ptr<ASTNode> valueNode_;
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
        std::string functionName_;
        std::vector<Parameter> parameters_;
        std::unique_ptr<SymbolNode> returnType_;
        std::vector<std::unique_ptr<ASTNode>> body_;
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
        std::string functionName_;
        std::vector<std::unique_ptr<ASTNode>> arguments_;
        FunctionCallNode(const std::string &functionName, std::vector<std::unique_ptr<ASTNode>> arguments);
        NodeType getType() const override;
        std::string toString() const override;
    };

    class VariableAssignmentNode : public ASTNode
    {
    public:
        std::string variableName_;
        std::unique_ptr<ASTNode> valueNode_;

        VariableAssignmentNode(const std::string &variableName, std::unique_ptr<ASTNode> valueNode);

        NodeType getType() const override;

        std::string toString() const override;
    };

    class ForIterationNode : public ASTNode
    {
    public:
        ForIterationNode(const std::string &index,
                         std::unique_ptr<ASTNode> start,
                         std::unique_ptr<ASTNode> end,
                         std::unique_ptr<ASTNode> step,
                         std::vector<std::unique_ptr<ASTNode>> body);

        NodeType getType() const override;
        std::string toString() const override;

        std::string index_;
        std::unique_ptr<ASTNode> start_;
        std::unique_ptr<ASTNode> end_;
        std::unique_ptr<ASTNode> step_;
        std::vector<std::unique_ptr<ASTNode>> body_;
    };

    class WhileIterationNode : public ASTNode
    {
    public:
        WhileIterationNode(std::unique_ptr<ASTNode> condition, std::vector<std::unique_ptr<ASTNode>> body);

        NodeType getType() const override;
        std::string toString() const override;

        std::unique_ptr<ASTNode> condition_;
        std::vector<std::unique_ptr<ASTNode>> body_;
    };

    class IfNode : public ASTNode
    {
    public:
        IfNode(std::unique_ptr<ASTNode> condition,
               std::unique_ptr<ASTNode> thenBranch,
               std::unique_ptr<ASTNode> elseBranch);

        NodeType getType() const override;
        std::string toString() const override;

        std::unique_ptr<ASTNode> condition_;
        std::unique_ptr<ASTNode> thenBranch_;
        std::unique_ptr<ASTNode> elseBranch_;
    };

    // Converts a NodeType to its string representation
    std::string ASTNodeTypeToString(NodeType type);
}