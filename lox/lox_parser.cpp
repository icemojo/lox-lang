#include "lox_parser.h"

ExprPtr 
Parser::parse()
{
    return expression();
    // catch (ParserError error) { return null; }
}

ExprPtr
Parser::expression()
{
    return equality();
}

ExprPtr 
Parser::equality()
{
    ExprPtr expr = comparision();
    while (match({ TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL })) {
        Token optr = previous();
        ExprPtr right = comparision();
        expr = make_binary_expr(
            std::move(std::visit(CopierVisiter{}, *expr)),
            optr, 
            std::move(std::visit(CopierVisiter{}, *right))
        );
    }
    return expr;
}

ExprPtr 
Parser::comparision()
{
    ExprPtr expr = term();
    while (match({ 
        TokenType::GREATER, TokenType::GREATER_EQUAL, 
        TokenType::LESS, TokenType::LESS_EQUAL })) 
    {
        Token optr = previous();
        ExprPtr right = term();
        expr = make_binary_expr(
            std::move(std::visit(CopierVisiter{}, *expr)),
            optr,
            std::move(std::visit(CopierVisiter{}, *right))
        );
    }
    return expr;
}

ExprPtr 
Parser::term()
{
    ExprPtr expr = factor();
    while (match({ TokenType::MINUS, TokenType::PLUS })) {
        Token optr = previous();
        ExprPtr right = factor();
        expr = make_binary_expr(
            std::move(std::visit(CopierVisiter{}, *expr)),
            optr,
            std::move(std::visit(CopierVisiter{}, *right))
        );
    }
    return expr;
}

ExprPtr 
Parser::factor()
{
    ExprPtr expr = unary();
    while (match({ TokenType::SLASH, TokenType::STAR })) {
        Token optr = previous();
        ExprPtr right = unary();
        expr = make_binary_expr(
            std::move(std::visit(CopierVisiter{}, *expr)),
            optr,
            std::move(std::visit(CopierVisiter{}, *right))
        );
    }
    return expr;
}

ExprPtr 
Parser::unary()
{
    if (match({ TokenType::BANG, TokenType::MINUS })) {
        Token optr = previous();
        ExprPtr right = unary();
        ExprPtr expr = make_unary(optr, std::move(std::visit(CopierVisiter{}, *right)));
        return expr;
    }
    return primary();
}

ExprPtr 
Parser::primary()
{
    if (match({ TokenType::FALSE })) {
        auto expr = make_literal("false");
        return expr;
    }
    if (match({ TokenType::TRUE })) {
        auto expr = make_literal("true");
        return expr;
    }
    if (match({ TokenType::NIL })) {
        auto expr = make_literal("nil");
        return expr;
    }

    if (match({ TokenType::NUMBER, TokenType::STRING })) {
        auto expr = make_literal(previous().literal);
        return expr;
    }

    if (match({ TokenType::LEFT_PAREN })) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        auto group_expr = make_grouping(std::move(expr));
        return group_expr;
    }

    // TODO(yemon): throw/return error(peek(), "Expect an expression.")
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
