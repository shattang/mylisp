#pragma once

#include "ASTNode.h"
#include <iostream>
#include <string>

namespace Shattang::MyLisp {

class ASTPrettyPrinter : public ASTVisitor {
public:
    ASTPrettyPrinter(std::ostream &out, int indentSize = 2);

    void visit(const ASTNode &node) override;
    void print(const ASTNode &node);

private:
    std::ostream &out_;
    int indentLevel_;
    int indentSize_;

    void indent();
    void visitNode(const ASTNode &node);
};

} // namespace Shattang::MyLisp
