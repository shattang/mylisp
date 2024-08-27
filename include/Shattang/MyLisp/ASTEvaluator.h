#pragma once

#include "ASTNode.h"
#include "TypeSystem.h"

namespace Shattang::MyLisp
{

    class Environment;

    class ASTEvaluator
    {
    public:
        Value evaluate(const ASTNode &node, Environment *env);
    };

} // namespace Shattang::MyLisp
