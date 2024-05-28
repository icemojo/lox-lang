#ifndef LOX_PARSER_
#define LOX_PARSER_

#include <vector>
#include <memory>
#include <utility>
#include "lox_lexer.h"
#include "lox_ast.h"

using std::vector;

class Parser {
public:
    Parser(const vector<Token> tokens) : 
        tokens(tokens)
    {}

private:
    const vector<Token> &tokens;
    size_t current = 0;

public:
    Expr expression();

    Expr equality();

    Expr comparision();

    Expr term();

    Expr factor();

    Expr unary();

    Expr primary();

private:
    bool match(std::initializer_list<TokenType> token_types);

    bool check(TokenType type);

    bool is_end();

    Token peek();

    Token previous();

    Token advance();

    Token consume(TokenType token_type, const string_view message);

};

struct ParserResult {
    bool has_error{ false };
    Token token_result{};
    string message{};
};

ParserResult error(Token token, const string_view message);

#endif  // LOX_PARSER_
