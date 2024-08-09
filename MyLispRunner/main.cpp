#include <iostream>
#include <vector>
#include <Shattang/MyLisp/Lexer.h>
#include <Shattang/MyLisp/Parser.h>

using namespace Shattang::MyLisp;

int main()
{
    // Example MyLisp script with a function definition, variable assignment,
    // conditional, and function call
    std::string myLispScript = R"(
        ;; Define the numbers using make-vector
        (let (numbers Vector)
            (make-vector 3.0E-50 7.2 5.8 6.1 8.0 4.4 5.9 6.8 7.3 4)
        )

        ;; Function to calculate the mean
        (define mean ((nums Vector)) Float
            (divide (reduce add 0 nums) (length nums))
        )

        ;; Function to calculate the variance
        (define variance ((nums Vector) (avg Float)) Float
            ;; Nested function to square the difference
            (define squareDifference ((x Float)) Float
                (multiply (subtract x avg) (subtract x avg))
            )
            (reduce add 0 (map squareDifference nums))
        )

        ;; Calculate the mean
        (let (avg Float)
            (mean numbers)
        )

        ;; Calculate the variance
        (let (varianceSum Float)
            (divide (variance numbers avg) (length numbers))
        )

        ;; Calculate the standard deviation
        (let (stdDev Float)
            (sqrt varianceSum)
        )

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

    auto lexer = Shattang::MyLisp::Lexer(myLispScript);
    auto parser = Shattang::MyLisp::Parser(lexer);

    auto ast = parser.parse();

    std::cout << ast->toString();
    
    return 0;
}
