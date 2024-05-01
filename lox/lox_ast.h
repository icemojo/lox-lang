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
    void operator()(BinaryExpr const &binary) const;

    void operator()(Grouping const &group) const;

    void operator()(Literal const &literal) const;

    void operator()(Unary const &unary) const;
};

struct CopyVisitor {
    ExprPtr operator()(BinaryExpr const &binary) const;

    ExprPtr operator()(Grouping const &group) const;

    ExprPtr operator()(Literal const &literal) const;

    ExprPtr operator()(Unary const &unary) const;
};

struct PrintVisitor {
    bool line_break;

    PrintVisitor() : line_break(false) {}
    PrintVisitor(bool line_break) : line_break(line_break) {}

    void operator()(BinaryExpr const &binary) const;

    void operator()(Grouping const &group) const;

    void operator()(Literal const &literal) const;

    void operator()(Unary const &unary) const;
};

//------------------------------------------------------------------------------

void test();
void test2();

#endif  // LOX_AST_