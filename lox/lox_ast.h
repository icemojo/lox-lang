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

//using Expr_V     = std::variant<BinaryExpr, Grouping, Literal, Unary>;
//using ExprPtr  = std::unique_ptr<Expr>;

union Expr_V {
    BinaryExpr binary;
    Grouping group;
    Literal literal;
    Unary unary;
};

enum class ExprType {
    NONE = 0,
    BINARY,
    GROUPING,
    LITERAL,
    UNARY,
};

struct Expr {
    Expr_V variant;
    ExprType type = ExprType::NONE;
};

struct BinaryExpr {
    Expr *left;
    Token optr;
    Expr *right;

    //BinaryExpr() : optr(Token{}) { left = nullptr; right = nullptr; }
    //BinaryExpr(const Expr *left, const Token optr, const Expr *right) : 
    //    left(left),
    //    optr(optr),
    //    right(right)
    //{}
};

struct Grouping {
    Expr *expr;
    
    //Grouping() { expr = nullptr; }
    //Grouping(Expr *expr) : expr(expr) {}
};

struct Literal {
    string value;  // NOTE(yemon): This *should* probably be a generic Object type... probably

    //Literal() : value("") {}
    //Literal(string_view value) : value(value) {}
};

struct Unary {
    Token optr;
    Expr *right;

    //Unary() : optr(Token{}) { right = nullptr; }
    //Unary(Token optr) : optr(optr) { right = nullptr; }
    //Unary(Token optr, Expr *right) : 
    //    optr(optr),
    //    right(right)
    //{}
};

//ExprPtr make_binary_expr(ExprPtr left, const Token optr, ExprPtr right);
Expr *make_binary_expr(Expr *left, const Token optr, Expr *right);

//ExprPtr make_grouping(ExprPtr expr);
Expr *make_grouping(Expr *expr);

//ExprPtr make_literal(const string &value);
Expr *make_literal(const string &value);

//ExprPtr make_unary(const Token optr, ExprPtr right);
Expr *make_unary(const Token optr, Expr *right);

//------------------------------------------------------------------------------

//struct ParserVisiter {
//    void operator()(const BinaryExpr &binary) const;
//
//    void operator()(const Grouping &group) const;
//
//    void operator()(const Literal &literal) const;
//
//    void operator()(const Unary &unary) const;
//};
//
//struct CopierVisiter {
//    Expr *operator()(const BinaryExpr &binary) const;
//
//    Expr *operator()(const Grouping &group) const;
//
//    Expr *operator()(const Literal &literal) const;
//
//    Expr *operator()(const Unary &unary) const;
//};

//struct PrinterVisiter {
//    bool line_break;
//
//    PrinterVisiter() : line_break(false) {}
//    PrinterVisiter(bool line_break) : line_break(line_break) {}
//
//    void operator()(const BinaryExpr &binary) const;
//
//    void operator()(const Grouping &group) const;
//
//    void operator()(const Literal &literal) const;
//
//    void operator()(const Unary &unary) const;
//};

void expr_print(const Expr *expr, bool line_break = false);

//------------------------------------------------------------------------------

//void test();
void test2();

#endif  // LOX_AST_