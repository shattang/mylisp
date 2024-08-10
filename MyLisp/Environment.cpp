#include <Shattang/MyLisp/Environment.h>

#include <stdexcept>

namespace Shattang::MyLisp
{

    // Function management
    void Environment::addFunction(const std::string &name, std::unique_ptr<Function> function)
    {
        functions_[name] = std::move(function);
    }

    Function &Environment::getFunction(const std::string &name)
    {
        auto it = functions_.find(name);
        if (it != functions_.end())
        {
            return *(it->second);
        }
        else
        {
            throw std::runtime_error("Function not found: " + name);
        }
    }

    // Variable management
    void Environment::setVariable(const std::string &name, const Value &value)
    {
        variables_[name] = value;
    }

    Value Environment::getVariable(const std::string &name) const
    {
        auto it = variables_.find(name);
        if (it != variables_.end())
        {
            return it->second;
        }
        else
        {
            throw std::runtime_error("Variable not found: " + name);
        }
    }

} // namespace Shattang::MyLisp
