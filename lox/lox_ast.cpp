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

void 
ParseVisitor::operator()(BinaryExpr const &binary) const
{
}

void 
ParseVisitor::operator()(Grouping const &group) const
{
}

void 
ParseVisitor::operator()(Literal const &literal) const
{
}

void 
ParseVisitor::operator()(Unary const &unary) const
{
}

ExprPtr 
CopyVisitor::operator()(BinaryExpr const &binary) const 
{
    auto new_binary = make_binary_expr(
        std::move(std::visit(CopyVisitor{}, *(binary.left))),
        binary.optr,
        std::move(std::visit(CopyVisitor{}, *(binary.right)))
    );
    return new_binary;
}

ExprPtr 
CopyVisitor::operator()(Grouping const &group) const 
{
    auto new_group = make_grouping(
        std::move(std::visit(CopyVisitor{}, *(group.expr)))
    );
    return new_group;
}

ExprPtr 
CopyVisitor::operator()(Literal const &literal) const 
{
    auto new_literal = make_literal(literal.value);
    return new_literal;
}

ExprPtr
CopyVisitor::operator()(Unary const &unary) const 
{
    auto new_unary = make_unary(
        unary.optr,
        std::move(std::visit(CopyVisitor{}, *(unary.right)))
    );
    return std::move(new_unary);
}

void 
PrintVisitor::operator()(BinaryExpr const &binary) const
{
    std::cout << "(" << binary.optr.lexeme << " ";
    std::visit(PrintVisitor{}, *(binary.left));
    std::cout << " ";
    std::visit(PrintVisitor{}, *(binary.right));
    std::cout << ")";
    if (line_break) {
        std::cout << '\n';
    }
}

void 
PrintVisitor::operator()(Grouping const &group) const 
{
    std::cout << "(";
    std::visit(PrintVisitor{}, *(group.expr));
    std::cout << ")";
    if (line_break) {
        std::cout << '\n';
    }
}

void 
PrintVisitor::operator()(Literal const &literal) const 
{
    std::cout << literal.value;
    if (line_break) {
        std::cout << '\n';
    }
}

void 
PrintVisitor::operator()(Unary const &unary) const 
{
    std::cout << "(" << unary.optr.lexeme << " ";
    if (unary.right) {
        std::visit(PrintVisitor{}, *(unary.right));
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

