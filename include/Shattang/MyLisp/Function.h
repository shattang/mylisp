#pragma once

#include "TypeSystem.h"

namespace Shattang::MyLisp
{
    struct FunctionParameter
    {
        std::string name_;
        ValueType type_;
    };

    using FunctionParameters = std::vector<FunctionParameter>; 

    class Function
    {
    public:
        virtual ~Function() = default;

        virtual FunctionParameter getParams() const;

        virtual ValueType getResultType() const;

        virtual Function &setArg(const Value& value) = 0;

        virtual Value execute() = 0;
    };

} // namespace Shattang::MyLisp
