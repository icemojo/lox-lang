#include <string.h>
#include "clox_common.h"
#include "clox_lexer.h"

const char *token_type_to_string(const Token_Type type)
{
    switch (type) {
    case TOKEN_EOL:
        return "EOL";
    case TOKEN_EOF:
        return "EOF";

    case TOKEN_LEFT_PAREN:
        return "LEFT_PAREN";
    case TOKEN_RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TOKEN_LEFT_BRACE:
        return "LEFT_BRACE";
    case TOKEN_RIGHT_BRACE:
        return "RIGHT_BRACE";
    case TOKEN_COMMA:
        return "COMMA";
    case TOKEN_DOT:
        return "DOT";
    case TOKEN_MINUS:
        return "MINUS";
    case TOKEN_PLUS:
        return "PLUS";
    case TOKEN_SEMICOLON:
        return "SEMICOLON";
    case TOKEN_SLASH:
        return "SLASH";
    case TOKEN_STAR:
        return "STAR";

    case TOKEN_BANG:
        return "BANG";
    case TOKEN_BANG_EQUAL:
        return "BANG_EQUAL";
    case TOKEN_EQUAL:
        return "EQUAL";
    case TOKEN_EQUAL_EQUAL:
        return "EQUAL_EQUALL";
    case TOKEN_GREATER:
        return "GREATER";
    case TOKEN_GREATER_EQUAL:
        return "GREATER_EQUAL";
    case TOKEN_LESS:
        return "LESS";
    case TOKEN_LESS_EQUAL:
        return "LESS_EQUAL";

    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";
    case TOKEN_STRING:
        return "STRING";
    case TOKEN_NUMBER:
        return "NUMBER";

    case TOKEN_AND:
        return "AND";
    case TOKEN_OR:
        return "OR";
    case TOKEN_VAR:
        return "VAR";
    case TOKEN_CLASS:
        return "CLASS";
    case TOKEN_FUNCTION:
        return "FUNCTION";
    case TOKEN_RETURN:
        return "RETURN";
    case TOKEN_IF:
        return "IF";
    case TOKEN_ELSE:
        return "ELSE";
    case TOKEN_FOR:
        return "FOR";
    case TOKEN_WHILE:
        return "WHILE";
    case TOKEN_TRUE:
        return "TRUE";
    case TOKEN_FALSE:
        return "FALSE";
    case TOKEN_NIL:
        return "NIL";
    case TOKEN_PRINT:
        return "PRINT";
    case TOKEN_SUPER:
        return "SUPER";
    case TOKEN_THIS:
        return "THIS";

    default:
        return "ILLEGAL";
    }
}

Token make_token(const Scanner *scanner, const Token_Type type)
{
    Token new_token  = {0};
    new_token.type   = type;
    new_token.start  = scanner->start;
    new_token.length = (int32_t)(scanner->current - scanner->start);
    new_token.line   = scanner->line;
    return new_token;
}

Token make_error_token(const char *message, const int32_t line)
{
    Token error_token  = {0};
    error_token.type   = TOKEN_ERROR;
    error_token.start  = message;
    error_token.length = (int32_t)strlen(message);
    error_token.line   = line;
    return error_token;
}

void init_scanner(Scanner *scanner, const char *source_line)
{
    scanner->start   = source_line;
    scanner->current = source_line;
    scanner->line    = 1;
}

Token scan_token(Scanner *scanner)
{
    scanner->start = scanner->current;
    if (scanner_is_end(scanner)) {
        return make_token(scanner, TOKEN_EOF);
    }
    else {
        //
    }
    return make_error_token("Unexpected character.", scanner->line);
}

bool scanner_is_end(const Scanner *scanner)
{
    return scanner->current == '\0';
}

