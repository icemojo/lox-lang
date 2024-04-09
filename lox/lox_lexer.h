#ifndef LOX_PARSER_
#define LOX_PARSER_

#include <string>
#include <vector>

using std::string;
using std::string_view;
using std::vector;

enum class TokenType {
    EOL = 0,
    EOF_ = 1,

    // Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    AND,
    OR,
    VAR,
    CLASS,
    FUNCTION,
    RETURN,
    IF,
    ELSE,
    FOR,
    WHILE,
    TRUE,
    FALSE,
    NIL,
    PRINT,
    SUPER,
    THIS
};

[[nodiscard]] string
TokenTypeToString(const TokenType &type);

//------------------------------------------------------------------------------

class Token {
public:
    Token(
        const TokenType type, 
        const string &lexeme, 
        const string &literal, 
        const uint32_t line
    ) :
        type(type),
        line(line)
    {}

    string to_string() const;

private:
    TokenType type;
    string lexeme;
    string literal;
    uint32_t line;
};

//------------------------------------------------------------------------------

class Scanner {
public:
    Scanner(const char *source) : 
        source(source),
        start(0),
        current(0),
        line(0)
    {}

    Scanner(const string_view &source) : 
        source(source),
        start(0),
        current(0),
        line(0)
    {}

    Scanner(const string &source) : 
        source(source),
        start(0),
        current(0),
        line(0)
    {}

    void scan_tokens();

private:
    string source;
    vector<Token> tokens;
    size_t start;
    size_t current;
    uint32_t line;

    void scan_token();

    [[nodiscard]] char advance();

    [[nodiscard]] char peek();

    [[nodiscard]] char peek_next();

    [[nodiscard]] bool match(char expected);

    void add_token(const TokenType &type);

    void add_token(const TokenType &type, const string &literal);

    void tokenize_string();

    void tokenize_number();

    [[nodiscard]] bool is_end() const;

};

#endif  // LOX_PARSER_
