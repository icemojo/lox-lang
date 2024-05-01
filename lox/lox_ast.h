#ifndef LOX_AST_
#define LOX_AST_

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include "lox_lexer.h"

struct BinaryExpr;
struct Grouping;
struct Literal;
struct Unary;

using Expr     = std::variant<BinaryExpr, Grouping, Literal, Unary>;
using ExprPtr  = std::unique_ptr<Expr>;

struct BinaryExpr {
    ExprPtr left;
    Token optr;
    ExprPtr right;

    BinaryExpr() : optr(Token{}) {}
    BinaryExpr(ExprPtr left, Token optr, ExprPtr right) : 
        left(std::move(left)),
        optr(optr),
        right(std::move(right))
    {}
};

struct Grouping {
    ExprPtr expr;
    
    Grouping() {}
    Grouping(ExprPtr expr) : expr(std::move(expr)) {}
};

struct Literal {
    std::string value;  // NOTE(yemon): This *should* probably be a generic Object type... probably

    Literal() : value("") {}
    Literal(std::string_view value) : value(value) {}
};

struct Unary {
    Token optr;
    ExprPtr right;

    Unary() : optr(Token{}) {}
    Unary(Token optr) : optr(optr) {}
    Unary(Token optr, ExprPtr right) : 
        optr(optr),
        right(std::move(right))
    {}
};

ExprPtr make_binary_expr(ExprPtr left, Token const optr, ExprPtr right);

ExprPtr make_grouping(ExprPtr expr);

ExprPtr make_literal(std::string const &value);

ExprPtr make_unary(Token const optr, ExprPtr right);

//------------------------------------------------------------------------------

struct ParseVisitor {
    void operator()(BinaryExpr const &binary) const {
    }

    void operator()(Grouping const &group) const {
    }

    void operator()(Literal const &literal) const { 
    }

    void operator()(Unary const &unary) const {
    }
};

struct CopyVisitor {
    ExprPtr operator()(BinaryExpr const &binary) const {
        auto new_binary = make_binary_expr(
            std::move(std::visit(CopyVisitor{}, *(binary.left))),
            binary.optr,
            std::move(std::visit(CopyVisitor{}, *(binary.right)))
        );
        return new_binary;
    }

    ExprPtr operator()(Grouping const &group) const {
        auto new_group = make_grouping(
            std::move(std::visit(CopyVisitor{}, *(group.expr)))
        );
        return new_group;
    }

    ExprPtr operator()(Literal const &literal) const { 
        auto new_literal = make_literal(literal.value);
        return new_literal;
    }

    ExprPtr operator()(Unary const &unary) const {
        auto new_unary = make_unary(
            unary.optr,
            std::move(std::visit(CopyVisitor{}, *(unary.right)))
        );
        return std::move(new_unary);
    }
};

struct PrintVisitor {
    bool line_break;

    PrintVisitor() : line_break(false) {}
    PrintVisitor(bool line_break) : line_break(line_break) {}

    void operator()(BinaryExpr const &binary) const {
        std::cout << "(" << binary.optr.lexeme << " ";
        std::visit(PrintVisitor{}, *(binary.left));
        std::cout << " ";
        std::visit(PrintVisitor{}, *(binary.right));
        std::cout << ")";
        if (line_break) {
            std::cout << '\n';
        }
    }

    void operator()(Grouping const &group) const {
        std::cout << "(";
        std::visit(PrintVisitor{}, *(group.expr));
        std::cout << ")";
        if (line_break) {
            std::cout << '\n';
        }
    }

    void operator()(Literal const &literal) const {
        std::cout << literal.value;
        if (line_break) {
            std::cout << '\n';
        }
    }

    void operator()(Unary const &unary) const {
        std::cout << "(" << unary.optr.lexeme << " ";
        if (unary.right) {
            std::visit(PrintVisitor{}, *(unary.right));
        }
        std::cout << ")";
        if (line_break) {
            std::cout << '\n';
        }
    }
};

//------------------------------------------------------------------------------

void test();
void test2();

#endif  // LOX_AST_