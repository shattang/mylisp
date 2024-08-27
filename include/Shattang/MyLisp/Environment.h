#pragma once


#include <Shattang/MyLisp/TypeSystem.h>
#include <Shattang/MyLisp/Function.h>
#include <Shattang/MyLisp/ASTNode.h>

#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>


namespace Shattang::MyLisp
{

    class Environment
    {
    public:
        virtual ~Environment() = default;

        // Constructor to initialize the environment with a parent and a type system
        Environment(Environment *parent, TypeSystem typeSystem);

        // Get the full name of the environment, including parent names
        virtual std::string getName() const;

        // Get a pointer to a variable's value, searching the parent environment if necessary
        virtual Value *getVariable(const std::string &name);

        // Set a variable in the current environment
        virtual void setVariable(const std::string &name, Value value);

        // Get a pointer to a function, searching the parent environment if necessary
        virtual Function *getFunction(const std::string &name);

        // Set a function in the current environment
        virtual void setFunction(const std::string &name, std::unique_ptr<Function> function);

        // Get a reference to the type system
        virtual TypeSystem &getTypeSystem();

        // Evaluate an ASTNode within the environment
        virtual Value evaluate(const ASTNode &node);

    protected:
        std::unordered_map<std::string, Value> variables_;
        std::unordered_map<std::string, std::unique_ptr<Function>> functions_;
        Environment *parent_;
        TypeSystem typeSystem_;
    };

} // namespace Shattang::MyLisp
