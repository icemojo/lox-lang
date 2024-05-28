#ifndef LOX_AST_
#define LOX_AST_

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include "lox_lexer.h"

using std::vector;
using std::string;
using std::string_view;

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
    string value;  // NOTE(yemon): This *should* probably be a generic Object type... probably

    Literal() : value("") {}
    Literal(string_view value) : value(value) {}
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

ExprPtr make_binary_expr(ExprPtr left, const Token optr, ExprPtr right);

ExprPtr make_grouping(ExprPtr expr);

ExprPtr make_literal(const string &value);

ExprPtr make_unary(const Token optr, ExprPtr right);

//------------------------------------------------------------------------------

struct ParserVisiter {
    void operator()(const BinaryExpr &binary) const;

    void operator()(const Grouping &group) const;

    void operator()(const Literal &literal) const;

    void operator()(const Unary &unary) const;
};

struct CopierVisiter {
    ExprPtr operator()(const BinaryExpr &binary) const;

    ExprPtr operator()(const Grouping &group) const;

    ExprPtr operator()(const Literal &literal) const;

    ExprPtr operator()(const Unary &unary) const;
};

struct PrinterVisiter {
    bool line_break;

    PrinterVisiter() : line_break(false) {}
    PrinterVisiter(bool line_break) : line_break(line_break) {}

    void operator()(const BinaryExpr &binary) const;

    void operator()(const Grouping &group) const;

    void operator()(const Literal &literal) const;

    void operator()(const Unary &unary) const;
};

//------------------------------------------------------------------------------

void test();
void test2();

#endif  // LOX_AST_