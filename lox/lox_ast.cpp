#include "lox_lexer.h"
#include "lox_ast.h"

ExprPtr 
make_binary_expr(ExprPtr left, Token const optr, ExprPtr right)
{
    auto binary = std::make_unique<Expr>(BinaryExpr{
        std::move(left),
        optr,
        std::move(right)
    });
    return binary;
}

ExprPtr 
make_grouping(ExprPtr expr)
{
    auto group = std::make_unique<Expr>(Grouping{
        std::move(expr)
    });
    return std::move(group);
}

ExprPtr
make_literal(std::string const &value) 
{
    auto literal = std::make_unique<Expr>(Literal{ value });
    return std::move(literal);
}

ExprPtr 
make_unary(Token const optr, ExprPtr right)
{
    auto unary = std::make_unique<Expr>(Unary{
        optr,
        std::move(right)
    });
    return std::move(unary);
}

//------------------------------------------------------------------------------

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
    ExprPtr lit1 = make_literal("123");
    std::visit(PrintVisitor{ true }, *lit1);

    ExprPtr left = make_unary(
        Token{ TokenType::MINUS, "-", "", 1 },
        std::move(std::visit(CopyVisitor{}, *lit1))
    );
    std::visit(PrintVisitor{ true }, *left);

    // (45.67)
    ExprPtr right = make_grouping(
        make_literal("45.67")
    );
    std::visit(PrintVisitor{ true }, *right);

    // (* (- 123) (45.67))
    ExprPtr expr = make_binary_expr(
        std::move(std::visit(CopyVisitor{}, *left)),
        Token{ TokenType::STAR, "*", "", 1 },
        std::move(std::visit(CopyVisitor{}, *right))
    );
    std::visit(PrintVisitor{ true }, *expr);

    ExprPtr expr2 = std::move(std::visit(CopyVisitor{}, *expr));
    std::visit(PrintVisitor{ true }, *expr2);
}

