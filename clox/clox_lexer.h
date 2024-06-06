#ifndef CLOX_LEXER_H_
#define CLOX_LEXER_H_

typedef enum token_type_e {
    TOKEN_EOL = 0,
    TOKEN_EOF = 1,

    // Single-character tokens
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,

    // One or two character tokens
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,

    // Literals
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,

    // Keywords
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_VAR,
    TOKEN_CLASS,
    TOKEN_FUNCTION,
    TOKEN_RETURN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NIL,
    TOKEN_PRINT,
    TOKEN_SUPER,
    TOKEN_THIS,

    TOKEN_ERROR,
    TOKEN_INVALID,      // TODO(yemon): Do I still need this?
} Token_Type;

typedef struct token_t {
    Token_Type type;
    const char *start;
    int32_t length;
    int32_t line;
} Token;

typedef struct scanner_t Scanner;

const char *token_type_to_string(const Token_Type type);

Token make_token(const Scanner *scanner, const Token_Type type);

Token make_error_token(const char *message, const int32_t line);

// NOTE(yemon): `start` points to the start of current lexeme,
// not the start of the entire source line.
// The scanner will only walk its way through the source once.
typedef struct scanner_t {
    const char *start;
    const char *current;
    int32_t line;
} Scanner;

void init_scanner(Scanner *scanner, const char *source_line);

Token scan_token(Scanner *scanner);

bool scanner_is_end(const Scanner *scanner);

#endif // CLOX_LEXER_H_
