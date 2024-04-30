#ifndef LOX_AST_
#define LOX_AST_

#include <string.h>
#include <variant>
#include "lox_lexer.h"

using string = std::string;
using string_view = std::string_view;

struct ExprVisitor;

struct Expr {
    virtual void accept(const ExprVisitor &visitor);
};

struct BinaryExpr : Expr {
    Expr left;
    Token optr;
    Expr right;

    BinaryExpr(Expr left, Token optr, Expr right) :
        left(left), optr(optr), right(right) 
    {}

    void accept(const ExprVisitor &visitor) override;
};

struct Grouping : Expr {
    Expr expression;
    
    Grouping(Expr expression) : expression(expression) {}

    void accept(const ExprVisitor &visitor) override;
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

//--------------------------------------------------------------------------------

struct ExprVisitor {
    virtual ~ExprVisitor() = default;
    virtual void visit(const BinaryExpr &expr) const = 0;
    virtual void visit(const Grouping &group) const = 0;
    virtual void visit(const Literal &literal) const = 0;
    virtual void visit(const Unary &unary) const = 0;
};

struct Parser : ExprVisitor {
    void visit(const BinaryExpr &expr) const override;
    void visit(const Grouping &group) const override;
    void visit(const Literal &literal) const override;
    void visit(const Unary &unary) const override;
};

#endif
