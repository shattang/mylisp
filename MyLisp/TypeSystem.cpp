#include <Shattang/MyLisp/TypeSystem.h>

namespace Shattang::MyLisp
{

    bool TypeSystem::IsType(const Value &value, ValueType type) const
    {
        switch (type)
        {
        case INTEGER:
            return std::holds_alternative<long>(value);
        case DOUBLE:
            return std::holds_alternative<double>(value);
        case STRING:
            return std::holds_alternative<std::string>(value);
        case BOOL:
            return std::holds_alternative<bool>(value);
        case INTEGER_VECTOR:
            return std::holds_alternative<IntegerVector>(value);
        case DOUBLE_VECTOR:
            return std::holds_alternative<DoubleVector>(value);
        case STRING_VECTOR:
            return std::holds_alternative<StringVector>(value);
        case BOOL_VECTOR:
            return std::holds_alternative<BoolVector>(value);
        default:
            return false;
        }
    }

    Value TypeSystem::ConvertTo(const Value &value, ValueType targetType) const
    {
        switch (targetType)
        {
        case DOUBLE:
            if (std::holds_alternative<long>(value))
            {
                return static_cast<double>(std::get<long>(value));
            }
            break;

        case DOUBLE_VECTOR:
            if (std::holds_alternative<IntegerVector>(value))
            {
                const auto &intVec = std::get<IntegerVector>(value);
                DoubleVector doubleVec(intVec.begin(), intVec.end());
                return doubleVec;
            }
            break;

        default:
            if (IsType(value, targetType))
            {
                return value;
            }
            break;
        }

        throw TypeConversionException(ValueToString(value), ValueTypeToString(targetType));
    }

    std::string TypeSystem::ValueTypeToString(ValueType type) const
    {
        switch (type)
        {
        case INTEGER:
            return "INTEGER";
        case DOUBLE:
            return "DOUBLE";
        case STRING:
            return "STRING";
        case BOOL:
            return "BOOL";
        case INTEGER_VECTOR:
            return "INTEGER_VECTOR";
        case DOUBLE_VECTOR:
            return "DOUBLE_VECTOR";
        case STRING_VECTOR:
            return "STRING_VECTOR";
        case BOOL_VECTOR:
            return "BOOL_VECTOR";
        default:
            return "UNKNOWN";
        }
    }

    std::string TypeSystem::ValueToString(const Value &value) const
    {
        if (std::holds_alternative<long>(value))
        {
            return "INTEGER";
        }
        else if (std::holds_alternative<double>(value))
        {
            return "DOUBLE";
        }
        else if (std::holds_alternative<std::string>(value))
        {
            return "STRING";
        }
        else if (std::holds_alternative<bool>(value))
        {
            return "BOOL";
        }
        else if (std::holds_alternative<IntegerVector>(value))
        {
            return "INTEGER_VECTOR";
        }
        else if (std::holds_alternative<DoubleVector>(value))
        {
            return "DOUBLE_VECTOR";
        }
        else if (std::holds_alternative<StringVector>(value))
        {
            return "STRING_VECTOR";
        }
        else if (std::holds_alternative<BoolVector>(value))
        {
            return "BOOL_VECTOR";
        }
        else
        {
            return "UNKNOWN";
        }
    }

} // namespace Shattang::MyLisp
