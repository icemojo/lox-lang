#ifndef LOX_PARSER_
#define LOX_PARSER_

#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::string_view;
using std::vector;
using std::unordered_map;

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
    THIS,

    INVALID,
};

[[nodiscard]] string
TokenTypeToString(const TokenType &type);

inline const unordered_map<string_view, TokenType>
KEYWORDS = {
    {"class", TokenType::CLASS},
    {"fun", TokenType::FUNCTION},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"nil", TokenType::NIL},
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE},
};

//------------------------------------------------------------------------------

class Token {
public:
    Token() : 
        type(TokenType::INVALID), 
        lexeme(""),
        literal(""),
        line(0) 
    {}
    Token(
        const TokenType type, 
        const string &lexeme, 
        const string &literal, 
        const uint32_t line
    ) :
        type(type),
        lexeme(lexeme),
        literal(literal),
        line(line)
    {}

    string lexeme;

    string to_string() const;

private:
    TokenType type;
    string literal;
    uint32_t line;
};

//------------------------------------------------------------------------------

class Scanner {
public:
    Scanner(const string_view &source) : 
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

    void tokenize_identifier();

    [[nodiscard]] bool is_end() const;

};

[[nodiscard]] bool is_alpha(const char c);

#endif  // LOX_PARSER_
