#include <iostream>
#include <vector>
#include <Shattang/MyLisp/lexer.h>

using namespace Shattang::MyLisp;

int main()
{
    // Example MyLisp script with a function definition, variable assignment,
    // conditional, and function call
    std::string myLispScript = R"(
        ;; Define the numbers using make-vector
        (define numbers (make-vector 3.0E-50 7.2 5.8 6.1 8.0 4.4 5.9 6.8 7.3 4))

        ;; Function to calculate the mean
        (define (mean nums)
            (divide (reduce add 0 nums) (length nums))
        )

        ;; Function to calculate the variance
        (define (variance nums avg)
            (reduce add 0 (map (lambda (x) (multiply (subtract x avg) (subtract x avg))) nums))
        )

        ;; Calculate the mean
        (define avg? (mean numbers))

        ;; Calculate the variance
        (define varianceSum (divide (variance numbers avg) (length numbers)))

        ;; Calculate the standard deviation
        (define stdDev (sqrt varianceSum))

        ;; Print result
        (print "Standard Deviation:" stdDev)
        )";

    std::vector<Token> tokens = Tokenize(myLispScript);

    // Print the tokens
    std::cout << "Tokens:\n";
    for (const Token &token : tokens)
    {
        std::cout << token.ToString() << "\n";
    }

    return 0;
}
