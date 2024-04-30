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

using Expr = std::variant<BinaryExpr, Grouping, Literal, Unary>;

struct BinaryExpr {
    std::unique_ptr<Expr> left;
    Token optr;
    std::unique_ptr<Expr> right;

    BinaryExpr() : optr(Token{}) {}
    BinaryExpr(std::unique_ptr<Expr> left, Token optr, std::unique_ptr<Expr> right) : 
        left(std::move(left)),
        optr(Token{}),
        right(std::move(right))
    {}
};

struct Grouping {
    std::unique_ptr<Expr> expr;
    
    Grouping() {}
    Grouping(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
};

struct Literal {
    std::string value;  // NOTE(yemon): This *should* probably be a generic Object type... probably

    Literal() : value("") {}
    Literal(std::string_view value) : value(value) {}
};

struct Unary {
    Token optr;
    std::unique_ptr<Expr> right;

    Unary() : optr(Token{}) {}
    Unary(Token optr, std::unique_ptr<Expr> right) : 
        optr(optr),
        right(std::move(right))
    {}
};

//--------------------------------------------------------------------------------

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

struct PrintVisitor {
    void operator()(BinaryExpr const &binary) const {
        std::cout << "(" << binary.optr.lexeme << " ";
        std::visit(PrintVisitor{}, *(binary.left));
        std::cout << " ";
        std::visit(PrintVisitor{}, *(binary.right));
        std::cout << ")";
    }

    void operator()(Grouping const &group) const {
        std::cout << "(";
        std::visit(PrintVisitor{}, *(group.expr));
        std::cout << ")";
    }

    void operator()(Literal const &literal) const {
        std::cout << literal.value;
    }

    void operator()(Unary const &unary) const {
        std::cout << "(" << unary.optr.lexeme << " ";
        std::visit(PrintVisitor{}, *(unary.right));
        std::cout << ")";
    }
};

#endif  // LOX_AST_