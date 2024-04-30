#include "lox_lexer.h"
#include "lox_ast.h"

static void test()
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

