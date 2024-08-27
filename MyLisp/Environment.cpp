#include <Shattang/MyLisp/Environment.h>

namespace Shattang::MyLisp
{
    Environment::Environment(const std::string &name, TypeSystem *typeSystem, ASTEvaluator *evaluator, Environment *parent)
        : name_(name), typeSystem_(typeSystem), astEvaluator_(evaluator), parent_(parent)
    {
        if (typeSystem_ == nullptr)
            throw std::invalid_argument("typeSystem is required");
        if (astEvaluator_ == nullptr)
            throw std::invalid_argument("astEvaluator is required");
    }

    Environment::Environment(const std::string &name, Environment *parent)
        : name_(name), parent_(parent)
    {
        if (!parent)
        {
            throw std::invalid_argument("Parent environment cannot be null.");
        }
        typeSystem_ = parent->getTypeSystem();
        astEvaluator_ = parent->getASTEvaluator();
    }

    void Environment::setVariable(const std::string &name, std::unique_ptr<Value> value)
    {
        if (!value)
        {
            throw std::invalid_argument("Attempted to set a null Value for variable: " + name);
        }
        variables_[name] = std::move(value);
    }

    Value *Environment::getVariable(const std::string &name)
    {
        auto it = variables_.find(name);
        if (it != variables_.end())
        {
            return it->second.get();
        }
        else if (parent_)
        {
            Value *parentValue = parent_->getVariable(name);
            if (!parentValue)
            {
                throw UndefinedSymbolException(name);
            }
            return parentValue;
        }
        else
        {
            throw UndefinedSymbolException(name);
        }
    }

    void Environment::setFunction(const std::string &name, std::unique_ptr<Function> function)
    {
        if (!function)
        {
            throw std::invalid_argument("Attempted to set a null Function for: " + name);
        }
        functions_[name] = std::move(function);
    }

    Function *Environment::getFunction(const std::string &name)
    {
        auto it = functions_.find(name);
        if (it != functions_.end())
        {
            return it->second.get();
        }
        else if (parent_)
        {
            Function *parentFunction = parent_->getFunction(name);
            if (!parentFunction)
            {
                throw UndefinedSymbolException(name);
            }
            return parentFunction;
        }
        else
        {
            throw UndefinedSymbolException(name);
        }
    }

    std::string Environment::getName() const
    {
        if (parent_)
        {
            return parent_->getName() + "." + name_;
        }
        else
        {
            return name_;
        }
    }

    TypeSystem *Environment::getTypeSystem() const
    {
        return typeSystem_;
    }

    ASTEvaluator *Environment::getASTEvaluator() const
    {
        return astEvaluator_;
    }
}
