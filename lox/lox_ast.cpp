#include <iostream>
#include "lox_lexer.h"
#include "lox_ast.h"

ExprPtr 
make_binary_expr(ExprPtr left, const Token optr, ExprPtr right)
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
make_literal(const string &value) 
{
    auto literal = std::make_unique<Expr>(Literal{ value });
    return std::move(literal);
}

ExprPtr 
make_unary(const Token optr, ExprPtr right)
{
    auto unary = std::make_unique<Expr>(Unary{
        optr,
        std::move(right)
    });
    return std::move(unary);
}

//------------------------------------------------------------------------------

void 
ParserVisiter::operator()(const BinaryExpr &binary) const
{
}

void 
ParserVisiter::operator()(const Grouping &group) const
{
}

void 
ParserVisiter::operator()(const Literal &literal) const
{
}

void 
ParserVisiter::operator()(const Unary &unary) const
{
}

ExprPtr 
CopierVisiter::operator()(const BinaryExpr &binary) const 
{
    auto new_binary = make_binary_expr(
        std::move(std::visit(CopierVisiter{}, *(binary.left))),
        binary.optr,
        std::move(std::visit(CopierVisiter{}, *(binary.right)))
    );
    return new_binary;
}

ExprPtr 
CopierVisiter::operator()(const Grouping &group) const 
{
    auto new_group = make_grouping(
        std::move(std::visit(CopierVisiter{}, *(group.expr)))
    );
    return new_group;
}

ExprPtr 
CopierVisiter::operator()(const Literal &literal) const 
{
    auto new_literal = make_literal(literal.value);
    return new_literal;
}

ExprPtr
CopierVisiter::operator()(const Unary &unary) const 
{
    auto new_unary = make_unary(
        unary.optr,
        std::move(std::visit(CopierVisiter{}, *(unary.right)))
    );
    return std::move(new_unary);
}

void 
PrinterVisiter::operator()(const BinaryExpr &binary) const
{
    std::cout << "(" << binary.optr.lexeme << " ";
    std::visit(PrinterVisiter{}, *(binary.left));
    std::cout << " ";
    std::visit(PrinterVisiter{}, *(binary.right));
    std::cout << ")";
    if (line_break) {
        std::cout << '\n';
    }
}

void 
PrinterVisiter::operator()(const Grouping &group) const 
{
    std::cout << "(";
    std::visit(PrinterVisiter{}, *(group.expr));
    std::cout << ")";
    if (line_break) {
        std::cout << '\n';
    }
}

void 
PrinterVisiter::operator()(const Literal &literal) const 
{
    std::cout << literal.value;
    if (line_break) {
        std::cout << '\n';
    }
}

void 
PrinterVisiter::operator()(const Unary &unary) const 
{
    std::cout << "(" << unary.optr.lexeme << " ";
    if (unary.right) {
        std::visit(PrinterVisiter{}, *(unary.right));
    }
    std::cout << ")";
    if (line_break) {
        std::cout << '\n';
    }
}

//------------------------------------------------------------------------------

void test()
{
    std::cout << "test(): -123\n";

    Expr binary = BinaryExpr{
        //.optr = Token{ TokenType::PLUS, "+", "+", 0 }
    };
    std::visit(ParserVisiter{}, binary);

    Expr grouping = Grouping{};
    std::visit(ParserVisiter{}, grouping);

    Expr literal = Literal{ "2002" };
    std::visit(ParserVisiter{}, literal);

    Expr unary = Unary{
        Token{ TokenType::MINUS, "-", "-", 1 },
        std::make_unique<Expr>(Literal{ "123" })
    };
    std::visit(PrinterVisiter{}, unary);

    std::cout << '\n';
}

void test2()
{
    std::cout << "test2(): -123 * 45.67\n";

    // (- 123)
    ExprPtr lit1 = make_literal("123");
    std::visit(PrinterVisiter{ true }, *lit1);

    ExprPtr left = make_unary(
        Token{ TokenType::MINUS, "-", "", 1 },
        std::move(std::visit(CopierVisiter{}, *lit1))
    );
    std::visit(PrinterVisiter{ true }, *left);

    // (45.67)
    ExprPtr right = make_grouping(
        make_literal("45.67")
    );
    std::visit(PrinterVisiter{ true }, *right);

    // (* (- 123) (45.67))
    ExprPtr expr = make_binary_expr(
        std::move(std::visit(CopierVisiter{}, *left)),
        Token{ TokenType::STAR, "*", "", 1 },
        std::move(std::visit(CopierVisiter{}, *right))
    );
    std::visit(PrinterVisiter{ true }, *expr);

    ExprPtr expr2 = std::move(std::visit(CopierVisiter{}, *expr));
    std::visit(PrinterVisiter{ true }, *expr2);

    std::cout << '\n';
}

