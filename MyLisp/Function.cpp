#include <Shattang/MyLisp/Function.h>
#include <Shattang/MyLisp/Environment.h>

namespace Shattang::MyLisp
{

    Function::Function(FunctionParameters params, ValueType returnType)
        : params_(std::move(params)), returnType_(returnType) {}

    FunctionParameters Function::getParams() const
    {
        return params_;
    }

    ValueType Function::getResultType() const
    {
        return returnType_;
    }

    Value Function::execute(Environment *env, const std::vector<Value> &arguments)
    {
        validateArguments(env, arguments);
        return doExecute(env, arguments);
    }

    void Function::validateArguments(Environment *env, const std::vector<Value> &arguments) const
    {
        if (params_.empty())
        {
            if (!arguments.empty())
            {
                throw std::runtime_error("Function does not take any arguments.");
            }
        }

        size_t numFixedParams = params_.size() - (params_.back().isVariadic_ ? 1 : 0);
        if (arguments.size() < numFixedParams)
        {
            throw std::runtime_error("Not enough arguments provided.");
        }

        if (!params_.back().isVariadic_ && arguments.size() > params_.size())
        {
            throw std::runtime_error("Too many arguments provided.");
        }

        TypeSystem *typeSystem = env->getTypeSystem();

        // Validate fixed arguments
        for (size_t i = 0; i < numFixedParams; ++i)
        {
            if (!typeSystem->IsType(arguments[i], params_[i].type_))
            {
                throw std::runtime_error("Argument type mismatch.");
            }
        }

        // Validate variadic arguments
        if (params_.back().isVariadic_)
        {
            for (size_t i = numFixedParams; i < arguments.size(); ++i)
            {
                if (!typeSystem->IsType(arguments[i], params_.back().type_))
                {
                    throw std::runtime_error("Variadic argument type mismatch.");
                }
            }
        }
    }

} // namespace Shattang::MyLisp
