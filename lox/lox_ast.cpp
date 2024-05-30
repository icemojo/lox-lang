#include <iostream>
#include "lox_lexer.h"
#include "lox_ast.h"

//ExprPtr 
//make_binary_expr(ExprPtr left, const Token optr, ExprPtr right)
Expr *
make_binary_expr(Expr *left, const Token optr, Expr *right)
{
    //auto binary = std::make_unique<Expr>(BinaryExpr{
    //    std::move(left),
    //    optr,
    //    std::move(right)
    //});
    Expr *binary = new Expr{
        .variant = BinaryExpr{
            .left = left,
            .optr = optr,
            .right = right,
        },
        .type = ExprType::BINARY,
    };
    return binary;
}

//ExprPtr 
//make_grouping(ExprPtr expr)
Expr *
make_grouping(Expr *expr)
{
    //auto group = std::make_unique<Expr>(Grouping{
    //    std::move(expr)
    //});
    Expr *group = new Expr{
        .variant = Grouping{
            .expr = expr
        },
        .type = ExprType::GROUPING,
    };
    return group;
}

//ExprPtr
Expr *
make_literal(const string &value) 
{
    //auto literal = std::make_unique<Expr>(Literal{ value });
    Expr *literal = new Expr{
        .variant = Literal{
            .value = value
        },
        .type = ExprType::LITERAL,
    };
    return literal;
}

//ExprPtr 
//make_unary(const Token optr, ExprPtr right)
Expr *
make_unary(const Token optr, Expr *right)
{
    //auto unary = std::make_unique<Expr>(Unary{
    //    optr,
    //    std::move(right)
    //});
    Expr *unary = new Expr{
        .variant = Unary{
            .optr = optr,
            .right = right,
        },
        .type = ExprType::UNARY,
    };
    return std::move(unary);
}

//------------------------------------------------------------------------------

//void 
//ParserVisiter::operator()(const BinaryExpr &binary) const
//{
//}
//
//void 
//ParserVisiter::operator()(const Grouping &group) const
//{
//}
//
//void 
//ParserVisiter::operator()(const Literal &literal) const
//{
//}
//
//void 
//ParserVisiter::operator()(const Unary &unary) const
//{
//}

//Expr *
//CopierVisiter::operator()(const BinaryExpr &binary) const 
//{
//    auto new_binary = make_binary_expr(
//        std::move(std::visit(CopierVisiter{}, *(binary.left))),
//        binary.optr,
//        std::move(std::visit(CopierVisiter{}, *(binary.right)))
//    );
//    return new_binary;
//}
//
//Expr *
//CopierVisiter::operator()(const Grouping &group) const 
//{
//    auto new_group = make_grouping(
//        std::move(std::visit(CopierVisiter{}, *(group.expr)))
//    );
//    return new_group;
//}
//
//Expr *
//CopierVisiter::operator()(const Literal &literal) const 
//{
//    auto new_literal = make_literal(literal.value);
//    return new_literal;
//}
//
//Expr *
//CopierVisiter::operator()(const Unary &unary) const 
//{
//    auto new_unary = make_unary(
//        unary.optr,
//        std::move(std::visit(CopierVisiter{}, *(unary.right)))
//    );
//    return std::move(new_unary);
//}

//void 
//PrinterVisiter::operator()(const BinaryExpr &binary) const
//{
//    std::cout << "(" << binary.optr.lexeme << " ";
//    std::visit(PrinterVisiter{}, *(binary.left));
//    std::cout << " ";
//    std::visit(PrinterVisiter{}, *(binary.right));
//    std::cout << ")";
//    if (line_break) {
//        std::cout << '\n';
//    }
//}
//
//void 
//PrinterVisiter::operator()(const Grouping &group) const 
//{
//    std::cout << "(";
//    std::visit(PrinterVisiter{}, *(group.expr));
//    std::cout << ")";
//    if (line_break) {
//        std::cout << '\n';
//    }
//}
//
//void 
//PrinterVisiter::operator()(const Literal &literal) const 
//{
//    std::cout << literal.value;
//    if (line_break) {
//        std::cout << '\n';
//    }
//}
//
//void 
//PrinterVisiter::operator()(const Unary &unary) const 
//{
//    std::cout << "(" << unary.optr.lexeme << " ";
//    if (unary.right) {
//        std::visit(PrinterVisiter{}, *(unary.right));
//    }
//    std::cout << ")";
//    if (line_break) {
//        std::cout << '\n';
//    }
//}

void expr_print(const Expr *expr, bool line_break)
{
    if (expr == nullptr) {
        return;
    }

    switch (expr->type) {
    case ExprType::BINARY: {
        //BinaryExpr binary = std::get<BinaryExpr>(expr->variant);
        BinaryExpr binary = expr->variant.binary;
        std::cout << "(" << binary.optr.lexeme << " ";
        expr_print(binary.left, line_break);
        std::cout << " ";
        expr_print(binary.right, line_break);
        std::cout << ")";
        if (line_break) {
            std::cout << '\n';
        }
    } break;

    case ExprType::GROUPING: {
        //Grouping group = std::get<Grouping>(expr->variant);
        Grouping group = expr->variant.group;
        std::cout << "(";
        expr_print(group.expr, line_break);
        std::cout << ")";
        if (line_break) {
            std::cout << '\n';
        }
    } break;

    case ExprType::LITERAL: {
        //Literal literal = std::get<Literal>(expr->variant);
        Literal literal = expr->variant.literal;
        std::cout << literal.value;
        if (line_break) {
            std::cout << '\n';
        }
    } break;

    case ExprType::UNARY: {
        //Unary unary = std::get<Unary>(expr->variant);
        Unary unary = expr->variant.unary;
        std::cout << "(" << unary.optr.lexeme << " ";
        if (unary.right) {
            expr_print(unary.right);
        }
        std::cout << ")";
        if (line_break) {
            std::cout << '\n';
        }
    } break;

    default:
        break;
    }
}

//------------------------------------------------------------------------------

//void test()
//{
//    std::cout << "test(): -123\n";
//
//    Expr binary = BinaryExpr{
//        //.optr = Token{ TokenType::PLUS, "+", "+", 0 }
//    };
//    std::visit(ParserVisiter{}, binary);
//
//    Expr grouping = Grouping{};
//    std::visit(ParserVisiter{}, grouping);
//
//    Expr literal = Literal{ "2002" };
//    std::visit(ParserVisiter{}, literal);
//
//    Expr unary = Unary{
//        Token{ TokenType::MINUS, "-", "-", 1 },
//        std::make_unique<Expr>(Literal{ "123" })
//    };
//    std::visit(PrinterVisiter{}, unary);
//
//    std::cout << '\n';
//}

//void test2()
//{
//    std::cout << "test2(): -123 * 45.67\n";
//
//    // (- 123)
//    ExprPtr lit1 = make_literal("123");
//    std::visit(PrinterVisiter{ true }, *lit1);
//
//    ExprPtr left = make_unary(
//        Token{ TokenType::MINUS, "-", "", 1 },
//        std::move(std::visit(CopierVisiter{}, *lit1))
//    );
//    std::visit(PrinterVisiter{ true }, *left);
//
//    // (45.67)
//    ExprPtr right = make_grouping(
//        make_literal("45.67")
//    );
//    std::visit(PrinterVisiter{ true }, *right);
//
//    // (* (- 123) (45.67))
//    ExprPtr expr = make_binary_expr(
//        std::move(std::visit(CopierVisiter{}, *left)),
//        Token{ TokenType::STAR, "*", "", 1 },
//        std::move(std::visit(CopierVisiter{}, *right))
//    );
//    std::visit(PrinterVisiter{ true }, *expr);
//
//    ExprPtr expr2 = std::move(std::visit(CopierVisiter{}, *expr));
//    std::visit(PrinterVisiter{ true }, *expr2);
//
//    std::cout << '\n';
//}

void test2()
{
    std::cout << "test3(): -123 * 45.67\n";

    // (- 123)
    Expr *lit1 = make_literal("123");
    expr_print(lit1, true);

    Expr *left = make_unary(Token{ TokenType::MINUS, "-", "", 1 }, lit1);
    expr_print(left, true);

    // (45.67)
    Expr *right = make_grouping(make_literal("45.67"));
    expr_print(right, true);

    // (* (-123) (45.67))
    Expr *expr = make_binary_expr(left, Token{ TokenType::STAR, "*", "", 1 }, right);
    expr_print(expr, true);

    Expr *expr2 = expr;
    expr_print(expr2, true);

    std::cout << '\n';
}

