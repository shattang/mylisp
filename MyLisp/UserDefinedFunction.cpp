#include <Shattang/MyLisp/UserDefinedFunction.h>
#include <Shattang/MyLisp/Environment.h>

namespace Shattang::MyLisp
{

    UserDefinedFunction::UserDefinedFunction(const std::string &functionName, FunctionParameters params, ValueType returnType, std::vector<std::unique_ptr<ASTNode>> body)
        : Function(std::move(params), returnType), functionName_(functionName), body_(std::move(body)) {}

    Value UserDefinedFunction::doExecute(Environment *env, const std::vector<Value> &arguments)
    {
        // Create a new local environment for the function execution with the function's name
        Environment localEnv(functionName_, env);

        // Bind the arguments to the function parameters in the local environment
        size_t numFixedParams = params_.size() - (params_.back().isVariadic_ ? 1 : 0);

        for (size_t i = 0; i < numFixedParams; ++i)
        {
            localEnv.setVariable(params_[i].name_, std::make_unique<Value>(arguments[i]));
        }

        // Handle variadic arguments if present
        if (params_.back().isVariadic_)
        {
            std::vector<Value> variadicArgs(arguments.begin() + numFixedParams, arguments.end());
            localEnv.setVariable(params_.back().name_, std::make_unique<Value>(variadicArgs));
        }

        // Execute each instruction in the function body using ASTEvaluator
        Value result;
        for (const auto &node : body_)
        {
            result = localEnv.getASTEvaluator()->evaluate(*node, &localEnv);
        }

        return result; // Return the result of the last expression
    }

} // namespace Shattang::MyLisp
