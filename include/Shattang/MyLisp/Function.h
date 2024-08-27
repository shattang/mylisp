#pragma once

#include "TypeSystem.h"
#include <vector>
#include <string>

namespace Shattang::MyLisp
{
    struct FunctionParameter
    {
        std::string name_;
        ValueType type_;
        bool isVariadic_ = false; // Flag to indicate if this parameter is variadic
    };

    using FunctionParameters = std::vector<FunctionParameter>;

    class Environment;

    class Function
    {
    public:
        // Constructor to initialize parameters and return type
        Function(FunctionParameters params, ValueType returnType);

        virtual ~Function() = default;

        // Returns the parameters of the function
        FunctionParameters getParams() const;

        // Returns the return type of the function
        ValueType getResultType() const;

        // Public virtual function to execute the function logic with validation
        virtual Value execute(Environment *env, const std::vector<Value> &arguments);

    protected:
        // Protected pure virtual method to perform the actual execution logic
        virtual Value doExecute(Environment *env, const std::vector<Value> &arguments) = 0;

        // Validates the arguments passed to the function
        void validateArguments(Environment *env, const std::vector<Value> &arguments) const;

        FunctionParameters params_; // Holds the parameters of the function
        ValueType returnType_;      // Holds the return type of the function
    };

} // namespace Shattang::MyLisp