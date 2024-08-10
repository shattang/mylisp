#pragma once

#include "Lexer.h"
#include "ASTNode.h"

namespace Shattang::MyLisp
{
    // Parser processes tokens from the Lexer to create an AST.
    class Parser
    {
    private:
        Lexer &lexer_;
        Token currentToken_;
        bool isParsingDefine_ = false;

        std::unique_ptr<ASTNode> parseExpression();
        std::unique_ptr<ASTNode> parseAtom();
        std::unique_ptr<ASTNode> parseLet();
        std::unique_ptr<ASTNode> parseDefine();
        std::unique_ptr<ASTNode> parseFunctionCall();
        std::unique_ptr<ASTNode> parseSet();
        std::unique_ptr<ASTNode> parseForIteration();
        std::unique_ptr<ASTNode> parseWhileIteration();
        std::unique_ptr<ASTNode> parseIf();
        void consume(TokenType expectedType);
        void throwError(const std::string &message);

    public:
        explicit Parser(Lexer &lexer);
        std::unique_ptr<ASTNode> parse();
    };

} // namespace Shattang::MyLisp
