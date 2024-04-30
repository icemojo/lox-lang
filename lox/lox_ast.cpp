#include "lox_lexer.h"
#include "lox_ast.h"

void test()
{
    Expr binary = BinaryExpr{
        //.optr = Token{ TokenType::PLUS, "+", "+", 0 }
    };
    std::visit(ParseVisitor{}, binary);

    Expr grouping = Grouping{};
    std::visit(ParseVisitor{}, grouping);

    Expr literal = Literal{ "2002" };
    std::visit(ParseVisitor{}, literal);

    Expr unary = Unary{
        Token{ TokenType::MINUS, "-", "-", 1 },
        std::make_unique<Expr>(Literal{ "123" })
    };
    std::visit(PrintVisitor{}, unary);

}

void test2()
{
    // (- 123)
    Expr left = Unary{
        Token{ TokenType::MINUS, "-", "", 1 },
        std::make_unique<Expr>(Literal{ "123" })
    };
    std::visit(PrintVisitor{}, left);
    std::cout << "\n";

    // (45.67)
    Expr right = Grouping{
        std::make_unique<Expr>(Literal{ "45.67" })
    };
    std::visit(PrintVisitor{}, right);
    std::cout << "\n";

    // (* (- 123) (45.67))
    Expr expr = BinaryExpr{
        std::make_unique<Expr>(Unary{
            Token{ TokenType::MINUS, "-", "", 1 }, 
            std::make_unique<Expr>(Literal{ "123" })
        }),
        Token{ TokenType::STAR, "*", "", 1 },
        std::make_unique<Expr>(Grouping{
            std::make_unique<Expr>(Literal{ "45.67" })
        })
    };
    std::visit(PrintVisitor{}, expr);
    std::cout << "\n";
}

