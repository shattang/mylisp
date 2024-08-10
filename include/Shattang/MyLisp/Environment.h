#pragma once

#include "Function.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <variant>

namespace Shattang::MyLisp
{
    class Environment
    {
    public:
        // Function management
        void addFunction(const std::string &name, std::unique_ptr<Function> function);
        Function &getFunction(const std::string &name);

        // Variable management
        void setVariable(const std::string &name, const Value &value);
        Value getVariable(const std::string &name) const;

    private:
        std::unordered_map<std::string, std::unique_ptr<Function>> functions_;
        std::unordered_map<std::string, Value> variables_;
    };

} // namespace Shattang::MyLisp
