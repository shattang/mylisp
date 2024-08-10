#include <Shattang/MyLisp/ASTPrettyPrinter.h>

namespace Shattang::MyLisp
{

    ASTPrettyPrinter::ASTPrettyPrinter(std::ostream &out, int indentSize)
        : out_(out), indentLevel_(0), indentSize_(indentSize) {}

    void ASTPrettyPrinter::indent()
    {
        out_ << std::string(indentLevel_ * indentSize_, ' ');
    }

    void ASTPrettyPrinter::print(const ASTNode &node)
    {
        node.visit(*this);
    }

    void ASTPrettyPrinter::visit(const ASTNode &node)
    {
        switch (node.getType())
        {
        case NodeType::SYMBOL:
        {
            const auto &symbolNode = static_cast<const SymbolNode &>(node);
            out_ << "(SYMBOL " << symbolNode.name_ << ")";
            break;
        }

        case NodeType::INTEGER:
        {
            const auto &intNode = static_cast<const IntegerNode &>(node);
            out_ << "(INTEGER " << intNode.value_ << ")";
            break;
        }

        case NodeType::FLOAT:
        {
            const auto &floatNode = static_cast<const FloatNode &>(node);
            out_ << "(FLOAT " << floatNode.value_ << ")";
            break;
        }

        case NodeType::BOOLEAN:
        {
            const auto &boolNode = static_cast<const BooleanNode &>(node);
            out_ << "(BOOLEAN " << (boolNode.value_ ? "true" : "false") << ")";
            break;
        }

        case NodeType::STRING:
        {
            const auto &stringNode = static_cast<const StringNode &>(node);
            out_ << "(STRING \"" << stringNode.value_ << "\")";
            break;
        }

        case NodeType::VARIABLE_DECLARATION:
        {
            const auto &varDeclNode = static_cast<const VariableDeclarationNode &>(node);
            indent();
            out_ << "(VARIABLE_DECLARATION (let (" << varDeclNode.variableName_ << " " << ASTNodeTypeToString(varDeclNode.typeNode_->getType()) << ") ";
            print(*varDeclNode.valueNode_);
            out_ << "))";
            break;
        }

        case NodeType::FUNCTION_DECLARATION:
        {
            indent();
            const auto &funcDeclNode = static_cast<const FunctionDeclarationNode &>(node);
            out_ << "(FUNCTION_DECLARATION (define " << funcDeclNode.functionName_ << " (";
            for (size_t i = 0; i < funcDeclNode.parameters_.size(); ++i)
            {
                const auto &param = funcDeclNode.parameters_[i];
                out_ << "(" << param.name_ << " " << param.type_->name_ << ")";
                if (i < funcDeclNode.parameters_.size() - 1)
                    out_ << " ";
            }
            out_ << ") " << funcDeclNode.returnType_->name_ << "\n";

            indentLevel_++;
            for (const auto &expr : funcDeclNode.body_)
            {
                print(*expr);
                out_ << "\n";
            }
            indentLevel_--;
            indent();
            out_ << "))";
            break;
        }

        case NodeType::FUNCTION_CALL:
        {
            indent();
            const auto &funcCallNode = static_cast<const FunctionCallNode &>(node);
            out_ << "(FUNCTION_CALL (" << funcCallNode.functionName_;
            for (const auto &arg : funcCallNode.arguments_)
            {
                out_ << " ";
                print(*arg);
            }
            out_ << "))";
            break;
        }

        case NodeType::VARIABLE_ASSIGNMENT:
        {
            indent();
            const auto &varAssignNode = static_cast<const VariableAssignmentNode &>(node);
            out_ << "(VARIABLE_ASSIGNMENT (set " << varAssignNode.variableName_ << " ";
            print(*varAssignNode.valueNode_);
            out_ << "))";
            break;
        }

        case NodeType::FOR_ITERATION:
        {
            indent();
            const auto &forNode = static_cast<const ForIterationNode &>(node);
            out_ << "(FOR_ITERATION (for " << forNode.index_ << " ";
            print(*forNode.start_);
            out_ << " ";
            print(*forNode.end_);
            out_ << " ";
            print(*forNode.step_);
            out_ << "\n";

            indentLevel_++;
            for (const auto &expr : forNode.body_)
            {
                print(*expr);
                out_ << "\n";
            }
            indentLevel_--;
            indent();
            out_ << "))";
            break;
        }

        case NodeType::WHILE_ITERATION:
        {
            indent();
            const auto &whileNode = static_cast<const WhileIterationNode &>(node);
            out_ << "(WHILE_ITERATION (while ";
            print(*whileNode.condition_);
            out_ << "\n";

            indentLevel_++;
            for (const auto &expr : whileNode.body_)
            {
                print(*expr);
                out_ << "\n";
            }
            indentLevel_--;
            indent();
            out_ << "))";
            break;
        }

        case NodeType::IF:
        {
            indent();
            const auto &ifNode = static_cast<const IfNode &>(node);
            out_ << "(IF (if ";
            print(*ifNode.condition_);
            out_ << "\n";

            indentLevel_++;
            indent();
            print(*ifNode.thenBranch_);
            out_ << "\n";
            indent();
            print(*ifNode.elseBranch_);
            indentLevel_--;
            out_ << "))";
            break;
        }

        case NodeType::SCRIPT:
        {
            const auto &scriptNode = static_cast<const ScriptNode &>(node);
            for (const auto &statement : scriptNode.statements_)
            {
                print(*statement);
                out_ << "\n";
            }
            break;
        }
        }
    }

} // namespace Shattang::MyLisp
