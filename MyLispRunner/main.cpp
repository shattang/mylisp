#include <iostream>
#include <vector>
#include <Shattang/MyLisp/Lexer.h>
#include <Shattang/MyLisp/Parser.h>
#include <Shattang/MyLisp/ASTPrettyPrinter.h>

using namespace Shattang::MyLisp;

int main()
{
    // Example MyLisp script with a function definition, variable assignment,
    // conditional, and function call
    std::string myLispScript = R"(

        (using "math")

        (let (numbers DoubleVector) (import-double-vector "data_source"))

        (define mean ((nums DoubleVector)) Float
            (let (sum Float) 0)
            (let (i Int) 0)
            (while (less-than i (length nums))
                (set sum (add sum (vector-ref nums i)))
                (set i (add i 1))
            )
            (divide sum (length nums))
        )

        (define variance ((nums DoubleVector) (avg Float)) Float
            (let (squaredNums DoubleVector) (make-double-vector))
            (for i 0 (subtract (length nums) 1) 1
                (vector-push squaredNums (multiply (vector-ref nums i) (vector-ref nums i)))
            )
            (let (meanOfSquaresValue Float) (mean squaredNums))
            (subtract meanOfSquaresValue (multiply avg avg))
        )

        (let (avg Float) (mean ((numbers))))

        (let (varianceSum Float) (variance numbers avg))

        (let (stdDev Float) (sqrt varianceSum))

        (let (overOne String) (if (greater-than stDev 1) "yes" "no"))

        (print "Standard Deviation:" stdDev)
        (print "OverOne?" overOne)

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

    ASTPrettyPrinter printer(std::cout);
    printer.print(*ast);
    
    return 0;
}
