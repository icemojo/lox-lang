#include <string.h>
#include "clox_common.h"
#include "clox_lexer.h"

static bool is_digit(const char c)
{
    return c >= '0' && c <= '9';
}

//------------------------------------------------------------------------------

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
    skip_whitespaces(scanner);

    scanner->start = scanner->current;
    if (scanner_is_end(scanner)) {
        return make_token(scanner, TOKEN_EOF);
    }

    char ch = scanner_advance(scanner);

    // NOTE(yemon): Handle the number literal case before anything else.
    if (is_digit(ch)) {
        return parse_number(scanner);
    }

    switch (ch) {
    case '(': 
        return make_token(scanner, TOKEN_LEFT_PAREN);
    case ')':
        return make_token(scanner, TOKEN_RIGHT_PAREN);
    case '{':
        return make_token(scanner, TOKEN_LEFT_BRACE);
    case '}':
        return make_token(scanner, TOKEN_RIGHT_BRACE);
    case ';':
        return make_token(scanner, TOKEN_SEMICOLON);
    case ',':
        return make_token(scanner, TOKEN_COMMA);
    case '.':
        return make_token(scanner, TOKEN_DOT);
    case '-':
        return make_token(scanner, TOKEN_MINUS);
    case '+':
        return make_token(scanner, TOKEN_PLUS);
    case '/':
        return make_token(scanner, TOKEN_SLASH);
    case '*':
        return make_token(scanner, TOKEN_STAR);

    case '!': {
        return scanner_match(scanner, '=') ?
            make_token(scanner, TOKEN_BANG_EQUAL) :
            make_token(scanner, TOKEN_BANG);
    }
    case '=': {
        return scanner_match(scanner, '=') ?
            make_token(scanner, TOKEN_EQUAL_EQUAL) :
            make_token(scanner, TOKEN_EQUAL);
    }
    case '<': {
        return scanner_match(scanner, '=') ?
            make_token(scanner, TOKEN_LESS_EQUAL) :
            make_token(scanner, TOKEN_LESS);
    }
    case '>': {
        return scanner_match(scanner, '=') ?
            make_token(scanner, TOKEN_GREATER_EQUAL) :
            make_token(scanner, TOKEN_GREATER);
    }

    case '"': {
        return parse_string(scanner);
    }
    }

    return make_error_token("Unexpected character.", scanner->line);
}

void skip_whitespaces(Scanner *scanner)
{
    for (;;) {
        const char next_ch = scanner_peek(scanner);
        switch (next_ch) {
        case ' ':
        case '\r':
        case '\t': {
            scanner_advance(scanner);
        } break;

        case '\n': {
            scanner->line += 1;
            scanner_advance(scanner);
        } break;

        case '/': {
            if (scanner_peek_next(scanner) == '/') {
                // consumes until the end of the current line
                while (scanner_peek(scanner) != '\n' && !scanner_is_end(scanner)) {
                    scanner_advance(scanner);
                }
            }
            else {
                return;
            }
        } break;

        default: return;
        }
    }
}

Token parse_string(Scanner *scanner)
{
    while (scanner_peek(scanner) != '"' && !scanner_is_end(scanner)) {
        // NOTE(yemon): Allowing multi-line strings.
        if (scanner_peek(scanner) == '\n') {
            scanner->line += 1;
            scanner_advance(scanner);
        }
    }

    if (scanner_is_end(scanner)) {
        return make_error_token("Unterminated string literal.", scanner->line);
    }

    // The closing quote.
    scanner_advance(scanner);
    return make_token(scanner, TOKEN_STRING);
}

Token parse_number(Scanner *scanner)
{
    while (is_digit(scanner_peek(scanner))) {
        scanner_advance(scanner);
    }

    // Look for a fractional point, which could be followed by more numbers.
    if (scanner_peek(scanner) == '.' && is_digit(scanner_peek_next(scanner))) {
        // consumes the '.'
        scanner_advance(scanner);

        while (is_digit(scanner_peek(scanner))) {
            scanner_advance(scanner);
        }
    }

    return make_token(scanner, TOKEN_NUMBER);
}

char scanner_advance(Scanner *scanner)
{
    // Consumes the current character and returns it.
    scanner->current += 1;
    return scanner->current[-1];
}

bool scanner_is_end(const Scanner *scanner)
{
    return scanner->current == '\0';
}

bool scanner_match(Scanner *scanner, const char expected)
{
    if (scanner_is_end(scanner)) {
        return false;
    }
    if (*(scanner->current) != expected) {
        return false;
    }
    // Just consumes the current character, in opposition to `_advance(..)`.
    scanner->current += 1;
    return true;
}

char scanner_peek(const Scanner *scanner)
{
    return *(scanner->current);
}

char scanner_peek_next(const Scanner *scanner)
{
    if (scanner_is_end(scanner)) { return '\0'; }
    return *(scanner->current+1);
}

