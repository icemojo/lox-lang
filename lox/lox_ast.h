#ifndef LOX_AST_
#define LOX_AST_

#include <string.h>
#include "lox_lexer.h"

using string = std::string;
using string_view = std::string_view;

struct Expr {
};

struct BinaryExpr : Expr {
    Expr left;
    Token optr;
    Expr right;

    BinaryExpr(Expr left, Token optr, Expr right) :
        left(left), optr(optr), right(right) 
    {}
};

struct Grouping : Expr {
    Expr expression;
    
    Grouping(Expr expression) : expression(expression) {}
};

struct Literal : Expr {
    // NOTE(yemon): This *should* probably be a generic Object type
    std::string value;

    Literal(string_view value) : value(value) {}
};

struct Unary : Expr {
    Token optr;
    Expr right;

    Unary(Token optr, Expr right) : optr(optr), right(right) {}
};

#endif
