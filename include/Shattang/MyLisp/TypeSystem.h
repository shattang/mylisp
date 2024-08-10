#pragma once

#include <variant>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

namespace Shattang::MyLisp
{

    enum ValueType
    {
        INTEGER,
        DOUBLE,
        STRING,
        BOOL,
        INTEGER_VECTOR,
        DOUBLE_VECTOR,
        STRING_VECTOR,
        BOOL_VECTOR
    };

    using IntegerVector = std::vector<long>;
    using DoubleVector = std::vector<double>;
    using StringVector = std::vector<std::string>;
    using BoolVector = std::vector<bool>;

    using Value = std::variant<long, double, std::string, bool, IntegerVector, DoubleVector, StringVector, BoolVector>;

    class TypeSystem
    {
    public:
        // Check if a Value matches a specific ValueType
        bool IsType(const Value &value, ValueType type) const;

        // Convert a Value to a target ValueType if possible
        Value ConvertTo(const Value &value, ValueType targetType) const;

        // Utility to get the name of the ValueType
        std::string ValueTypeToString(ValueType type) const;

        // Utility to get the type of a Value as a string
        std::string ValueToString(const Value &value) const;
    };

    // Custom exception for type conversion errors
    class TypeConversionException : public std::runtime_error
    {
    public:
        TypeConversionException(const std::string &fromType, const std::string &toType)
            : std::runtime_error("Cannot convert from " + fromType + " to " + toType) {}
    };

} // namespace Shattang::MyLisp
