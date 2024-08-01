#include <iostream>
#include <vector>
#include <Shattang/MyLisp/lexer.h>

using namespace Shattang::MyLisp;

int main() {
    Lexer lexer;

    // Example MyLisp script with a function definition, variable assignment, 
    // conditional, and function call
    std::string myLispScript = R"(
    (define factorial (lambda (n) (if (< n 2) 1 (* n (factorial (- n 1))))))
    (let x 5)
    (let x 15.001)
    (let x -15.001)
    (let x +15.001)
    (+ 17.001 -0.01)
    (for i 0 10 -1 
        (let x (+ x 1)))
    (if (> x 0)
        (print (factorial x))
        (print "x is not positive"))
    )"; 

    std::vector<Token> tokens = lexer.Tokenize(myLispScript);

    // Print the tokens
    std::cout << "Tokens:\n";
    for (const Token& token : tokens) {
        std::cout << token.ToString() << "\n";
    }

    return 0;
}
