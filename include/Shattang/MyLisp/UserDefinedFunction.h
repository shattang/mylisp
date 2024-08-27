#pragma once

#include "Function.h"
#include "ASTNode.h"
#include <vector>
#include <memory>

namespace Shattang::MyLisp
{
    class UserDefinedFunction : public Function
    {
    public:
        // Constructor to initialize the function parameters, return type, and body
        UserDefinedFunction(const std::string &functionName, FunctionParameters params, ValueType returnType, std::vector<std::unique_ptr<ASTNode>> body);

    protected:
        // The actual execution logic for the user-defined function
        Value doExecute(Environment *env, const std::vector<Value> &arguments) override;

    private:
        std::string functionName_;                   // The name of the function
        std::vector<std::unique_ptr<ASTNode>> body_; // The body of the function represented as AST nodes
    };

} // namespace Shattang::MyLisp
