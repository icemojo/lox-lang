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
Parser::operator()(const BinaryExpr &binary) const
{
}

void 
Parser::operator()(const Grouping &group) const
{
}

void 
Parser::operator()(const Literal &literal) const
{
}

void 
Parser::operator()(const Unary &unary) const
{
}

ExprPtr 
Copier::operator()(const BinaryExpr &binary) const 
{
    auto new_binary = make_binary_expr(
        std::move(std::visit(Copier{}, *(binary.left))),
        binary.optr,
        std::move(std::visit(Copier{}, *(binary.right)))
    );
    return new_binary;
}

ExprPtr 
Copier::operator()(const Grouping &group) const 
{
    auto new_group = make_grouping(
        std::move(std::visit(Copier{}, *(group.expr)))
    );
    return new_group;
}

ExprPtr 
Copier::operator()(const Literal &literal) const 
{
    auto new_literal = make_literal(literal.value);
    return new_literal;
}

ExprPtr
Copier::operator()(const Unary &unary) const 
{
    auto new_unary = make_unary(
        unary.optr,
        std::move(std::visit(Copier{}, *(unary.right)))
    );
    return std::move(new_unary);
}

void 
Printer::operator()(const BinaryExpr &binary) const
{
    std::cout << "(" << binary.optr.lexeme << " ";
    std::visit(Printer{}, *(binary.left));
    std::cout << " ";
    std::visit(Printer{}, *(binary.right));
    std::cout << ")";
    if (line_break) {
        std::cout << '\n';
    }
}

void 
Printer::operator()(const Grouping &group) const 
{
    std::cout << "(";
    std::visit(Printer{}, *(group.expr));
    std::cout << ")";
    if (line_break) {
        std::cout << '\n';
    }
}

void 
Printer::operator()(const Literal &literal) const 
{
    std::cout << literal.value;
    if (line_break) {
        std::cout << '\n';
    }
}

void 
Printer::operator()(const Unary &unary) const 
{
    std::cout << "(" << unary.optr.lexeme << " ";
    if (unary.right) {
        std::visit(Printer{}, *(unary.right));
    }
    std::cout << ")";
    if (line_break) {
        std::cout << '\n';
    }
}

//------------------------------------------------------------------------------

void test()
{
    Expr binary = BinaryExpr{
        //.optr = Token{ TokenType::PLUS, "+", "+", 0 }
    };
    std::visit(Parser{}, binary);

    Expr grouping = Grouping{};
    std::visit(Parser{}, grouping);

    Expr literal = Literal{ "2002" };
    std::visit(Parser{}, literal);

    Expr unary = Unary{
        Token{ TokenType::MINUS, "-", "-", 1 },
        std::make_unique<Expr>(Literal{ "123" })
    };
    std::visit(Printer{}, unary);

}

void test2()
{
    // (- 123)
    ExprPtr lit1 = make_literal("123");
    std::visit(Printer{ true }, *lit1);

    ExprPtr left = make_unary(
        Token{ TokenType::MINUS, "-", "", 1 },
        std::move(std::visit(Copier{}, *lit1))
    );
    std::visit(Printer{ true }, *left);

    // (45.67)
    ExprPtr right = make_grouping(
        make_literal("45.67")
    );
    std::visit(Printer{ true }, *right);

    // (* (- 123) (45.67))
    ExprPtr expr = make_binary_expr(
        std::move(std::visit(Copier{}, *left)),
        Token{ TokenType::STAR, "*", "", 1 },
        std::move(std::visit(Copier{}, *right))
    );
    std::visit(Printer{ true }, *expr);

    ExprPtr expr2 = std::move(std::visit(Copier{}, *expr));
    std::visit(Printer{ true }, *expr2);
}

