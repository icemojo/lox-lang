#ifndef LOX_AST_
#define LOX_AST_

#include <string.h>
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

    //BinaryExpr(Expr left, Token optr, Expr right) :
    //    left(left), optr(optr), right(right) 
    //{}
};

struct Grouping {
    std::unique_ptr<Expr> expr;
    
    //Grouping(Expr expr) : expr(expr) {}
};

struct Literal {
    // NOTE(yemon): This *should* probably be a generic Object type
    std::string value;

    //Literal(std::string_view value) : value(value) {}
};

struct Unary {
    Token optr;
    std::unique_ptr<Expr> right;

    //Unary(Token optr, Expr right) : optr(optr), right(right) {}
};

//--------------------------------------------------------------------------------

struct ParseVisitor {
    void operator()(const BinaryExpr &binary) const {
        // do stuff on the binary expr
    }

    void operator()(const Grouping &group) const {
        // do stuff on the expr group
    }

    void operator()(const Literal &literal) const { 
        // do stuff on the literal value
    }

    void operator()(const Unary &unary) const {
        // do stuff on the unary operator
    }
};

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

}

#endif  // LOX_AST_