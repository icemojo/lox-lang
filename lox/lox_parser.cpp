#include "lox_parser.h"

ExprPtr 
Parser::parse()
{
    return std::make_unique<Expr>();        // TODO(yemon): TEMP!
    //return expression();
    // catch (ParserError error) { return null; }
}

Expr
Parser::expression()
{
    return equality();
}

Expr 
Parser::equality()
{
    Expr expr = comparision();
    while (match({ TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL })) {
        Token optr = previous();
        Expr right = comparision();
        // TODO(yemon): expr = make_binary_expr(expr, optr, right);
    }
    return expr;
}

Expr 
Parser::comparision()
{
    Expr expr = term();
    while (match({ 
        TokenType::GREATER, TokenType::GREATER_EQUAL, 
        TokenType::LESS, TokenType::LESS_EQUAL })) 
    {
        Token optr = previous();
        Expr right = term();
        // TODO(yemon): expr = make_binary_expr(expr, optr, right);
    }
    return expr;
}

Expr 
Parser::term()
{
    Expr expr = factor();
    while (match({ TokenType::MINUS, TokenType::PLUS })) {
        Token optr = previous();
        Expr right = factor();
        // TODO(yemon): expr = make_binary_expr(expr, optr, right);
    }
    return expr;
}

Expr 
Parser::factor()
{
    Expr expr = unary();
    while (match({ TokenType::SLASH, TokenType::STAR })) {
        Token optr = previous();
        Expr right = unary();
        // TODO(yemon): expr = make_binary_expr(expr, optr, right);
    }
    return expr;
}

Expr 
Parser::unary()
{
    if (match({ TokenType::BANG, TokenType::MINUS })) {
        Token optr = previous();
        Expr right = unary();
        // TODO(yemon): expr = make_unary(optr, right);
    }
    return primary();
}

Expr 
Parser::primary()
{
    Expr temp{};
    if (match({ TokenType::FALSE })) {
        // TODO(yemon): expr = make_literal("false");
        return temp;
    }
    if (match({ TokenType::TRUE })) {
        // TODO(yemon): expr = make_literal("true");
        return temp;
    }
    if (match({ TokenType::NIL })) {
        // TODO(yemon): expr = make_literal("nil");
        return temp;
    }

    if (match({ TokenType::NUMBER, TokenType::STRING })) {
        // TODO(yemon): expr = make_literal(previous().literal);
        return temp;
    }

    if (match({ TokenType::LEFT_PAREN })) {
        Expr expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        // TODO(yemon): expr = make_grouping(expr);
        return temp;
    }

    // throw/return error(peek(), "Expect an expression.")
}

//------------------------------------------------------------------------------

bool 
Parser::match(std::initializer_list<TokenType> token_types)
{
    for (const auto &token_type : token_types) {
        if (check(token_type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool 
Parser::check(TokenType type)
{
    if (is_end()) { return false; }
    return peek().type == type;
}

bool 
Parser::is_end()
{
    return peek().type == TokenType::EOF_;
}

Token 
Parser::peek()
{
    return tokens.at(current);
}

Token 
Parser::previous()
{
    return tokens.at(current - 1);
}

Token 
Parser::advance()
{
    if (!is_end()) { current += 1; }
    return previous();
}

Token 
Parser::consume(TokenType token_type, const string_view message)
{
    if (check(token_type)) { return advance(); }
    auto result = error(peek(), message);
    return result.token_result;
}

void 
Parser::synchronize()
{
    advance();

    while (!is_end()) {
        if (previous().type == TokenType::SEMICOLON) { return; }

        switch (peek().type) {
        case TokenType::CLASS:
        case TokenType::FUNCTION:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
        }

        advance();
    }
}

ParserResult error(Token token, const string_view message)
{
    // TODO(yemon): ThrowError(token, message); ???
    return ParserResult{
        .has_error = true,
        .token_result = token,
        .message = string{ message }
    };
}
