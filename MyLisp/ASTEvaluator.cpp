#include <Shattang/MyLisp/ASTEvaluator.h>
#include <Shattang/MyLisp/Environment.h>
#include <Shattang/MyLisp/UserDefinedFunction.h>

namespace Shattang::MyLisp
{

    Value ASTEvaluator::evaluate(const ASTNode &node, Environment *env)
    {
        switch (node.getType())
        {
        case NodeType::SYMBOL:
        {
            const auto &symbolNode = static_cast<const SymbolNode &>(node);
            Value *value = env->getVariable(symbolNode.name_);
            if (value)
            {
                return *value;
            }
            else
            {
                throw std::runtime_error("Undefined symbol: " + symbolNode.name_);
            }
        }
        case NodeType::INTEGER:
        {
            const auto &intNode = static_cast<const IntegerNode &>(node);
            return intNode.value_;
        }
        case NodeType::FLOAT:
        {
            const auto &floatNode = static_cast<const FloatNode &>(node);
            return floatNode.value_;
        }
        case NodeType::BOOLEAN:
        {
            const auto &boolNode = static_cast<const BooleanNode &>(node);
            return boolNode.value_;
        }
        case NodeType::STRING:
        {
            const auto &stringNode = static_cast<const StringNode &>(node);
            return stringNode.value_;
        }
        case NodeType::VARIABLE_DECLARATION:
        {
            const auto &varDeclNode = static_cast<const VariableDeclarationNode &>(node);
            Value value = evaluate(*varDeclNode.valueNode_, env);
            env->setVariable(varDeclNode.variableName_, std::make_unique<Value>(std::move(value)));
            return Value(); // No specific return value for variable declaration
        }
        case NodeType::VARIABLE_ASSIGNMENT:
        {
            const auto &varAssignNode = static_cast<const VariableAssignmentNode &>(node);
            Value value = evaluate(*varAssignNode.valueNode_, env);
            Value *existingValue = env->getVariable(varAssignNode.variableName_);
            if (existingValue)
            {
                *existingValue = std::move(value);
            }
            else
            {
                throw std::runtime_error("Undefined variable: " + varAssignNode.variableName_);
            }
            return Value(); // No specific return value for variable assignment
        }
        case NodeType::FUNCTION_DECLARATION:
        {
            const auto &funcDeclNode = static_cast<const FunctionDeclarationNode &>(node);
            auto function = std::make_unique<UserDefinedFunction>(
                funcDeclNode.functionName_,
                funcDeclNode.parameters_,
                funcDeclNode.returnType_->getType(),
                funcDeclNode.body_);
            env->setFunction(funcDeclNode.functionName_, std::move(function));
            return Value(); // No specific return value for function declaration
        }
        case NodeType::FUNCTION_CALL:
        {
            const auto &funcCallNode = static_cast<const FunctionCallNode &>(node);
            Function *func = env->getFunction(funcCallNode.functionName_);
            if (!func)
            {
                throw std::runtime_error("Undefined function: " + funcCallNode.functionName_);
            }

            std::vector<Value> arguments;
            for (const auto &arg : funcCallNode.arguments_)
            {
                arguments.push_back(evaluate(*arg, env));
            }

            return func->execute(env, arguments);
        }
        case NodeType::IF:
        {
            const auto &ifNode = static_cast<const IfNode &>(node);
            Value conditionValue = evaluate(*ifNode.condition_, env);
            if (std::get<bool>(conditionValue))
            {
                return evaluate(*ifNode.thenBranch_, env);
            }
            else
            {
                return evaluate(*ifNode.elseBranch_, env);
            }
        }
        case NodeType::FOR_ITERATION:
        {
            const auto &forNode = static_cast<const ForIterationNode &>(node);
            Value startValue = evaluate(*forNode.start_, env);
            Value endValue = evaluate(*forNode.end_, env);
            Value stepValue = evaluate(*forNode.step_, env);

            if (!std::holds_alternative<long>(startValue) ||
                !std::holds_alternative<long>(endValue) ||
                !std::holds_alternative<long>(stepValue))
            {
                throw std::runtime_error("For loop bounds and step must be integers.");
            }

            long start = std::get<long>(startValue);
            long end = std::get<long>(endValue);
            long step = std::get<long>(stepValue);

            Value result;
            for (long i = start; i < end; i += step)
            {
                env->setVariable(forNode.index_, std::make_unique<Value>(i));
                for (const auto &expr : forNode.body_)
                {
                    result = evaluate(*expr, env);
                }
            }
            return result;
        }
        case NodeType::WHILE_ITERATION:
        {
            const auto &whileNode = static_cast<const WhileIterationNode &>(node);
            Value result;
            while (std::get<bool>(evaluate(*whileNode.condition_, env)))
            {
                for (const auto &expr : whileNode.body_)
                {
                    result = evaluate(*expr, env);
                }
            }
            return result;
        }
        default:
            throw std::runtime_error("Unknown ASTNode type.");
        }
    }

} // namespace Shattang::MyLisp
