#include <stdexcept>
#include "lox_lexer.h"

#ifdef _DEBUG
#include <type_traits>
#endif

[[nodiscard]] string
TokenTypeToString(const TokenType &type)
{
    switch (type) {
    case TokenType::EOL:
        return "EOL";
    case TokenType::EOF_:
        return "EOF";

    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TokenType::LEFT_BRACE:
        return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE:
        return "RIGHT_BRACE";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::DOT:
        return "DOT";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::SLASH:
        return "SLASH";
    case TokenType::STAR:
        return "STAR";

    case TokenType::BANG:
        return "BANG";
    case TokenType::BANG_EQUAL:
        return "BANG_EQUAL";
    case TokenType::EQUAL:
        return "EQUAL";
    case TokenType::EQUAL_EQUAL:
        return "EQUAL_EQUALL";
    case TokenType::GREATER:
        return "GREATER";
    case TokenType::GREATER_EQUAL:
        return "GREATER_EQUAL";
    case TokenType::LESS:
        return "LESS";
    case TokenType::LESS_EQUAL:
        return "LESS_EQUAL";

    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::STRING:
        return "STRING";
    case TokenType::NUMBER:
        return "NUMBER";

    case TokenType::AND:
        return "AND";
    case TokenType::OR:
        return "OR";
    case TokenType::VAR:
        return "VAR";
    case TokenType::CLASS:
        return "CLASS";
    case TokenType::FUNCTION:
        return "FUNCTION";
    case TokenType::RETURN:
        return "RETURN";
    case TokenType::IF:
        return "IF";
    case TokenType::ELSE:
        return "ELSE";
    case TokenType::FOR:
        return "FOR";
    case TokenType::WHILE:
        return "WHILE";
    case TokenType::TRUE:
        return "TRUE";
    case TokenType::FALSE:
        return "FALSE";
    case TokenType::NIL:
        return "NIL";
    case TokenType::PRINT:
        return "PRINT";
    case TokenType::SUPER:
        return "SUPER";
    case TokenType::THIS:
        return "THIS";

    default:
        return "ILLEGAL";
    }
}

//------------------------------------------------------------------------------

[[nodiscard]] string
Token::to_string() const
{
    const auto token_type = TokenTypeToString(type);
    string output = token_type + " ";
    if (lexeme.size() > 0) {
        output += lexeme;
    }

#ifdef _DEBUG
    static_assert(std::is_same<decltype(literal), std::string>::value, "Type of member 'literal' is not std::string.");
#endif
    if (literal.size() > 0) {
        output += literal;
    }
    return token_type + " " + lexeme + " " + literal;
}

//------------------------------------------------------------------------------

vector<Token> 
Scanner::get_tokens() const
{
    return tokens;
}

void 
Scanner::scan_tokens()
{
    while (!is_end()) {
        start = current;
        scan_token();
    }

    add_token(TokenType::EOF_, "", "");
}

void 
Scanner::scan_token()
{
    char next_ch = advance();
    switch (next_ch) {
    case '(':
        add_token(TokenType::LEFT_PAREN);
        break;
    case ')':
        add_token(TokenType::RIGHT_PAREN);
        break;
    case '{':
        add_token(TokenType::LEFT_BRACE);
        break;
    case '}':
        add_token(TokenType::RIGHT_BRACE);
        break;
    case ',':
        add_token(TokenType::COMMA);
        break;
    case '.':
        add_token(TokenType::DOT);
        break;
    case '-':
        add_token(TokenType::MINUS);
        break;
    case '+':
        add_token(TokenType::PLUS);
        break;
    case '*':
        add_token(TokenType::STAR);
        break;
    case ';':
        add_token(TokenType::SEMICOLON);
        break;

    case '!': {
        const TokenType type = match_next('=') ? TokenType::BANG_EQUAL : TokenType::BANG;
        const string_view lexeme = match_next('=') ? "!=" : "!";
        add_token(type, lexeme, "");
        [[maybe_unused]] char _ = advance();
    } break;

    case '=': {
        const TokenType type = match_next('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL;
        const string_view lexeme = match_next('=') ? "==" : "=";
        add_token(type, lexeme, "");
        [[maybe_unused]] char _ = advance();
    } break;

    case '<': {
        const TokenType type = match_next('=') ? TokenType::LESS_EQUAL : TokenType::LESS;
        const string_view lexeme = match_next('=') ? "<=" : "<";
        add_token(type, lexeme, "");
        [[maybe_unused]] char _ = advance();
    } break;

    case '>': {
        const TokenType type = match_next('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER;
        const string_view lexeme = match_next('=') ? ">=" : ">";
        add_token(type, lexeme, "");
        [[maybe_unused]] char _ = advance();
    } break;

    case '/': {
        if (match_next('/')) {
            while (peek() != '\n' && !is_end()) {
                [[maybe_unused]] char _ = advance();
            }
        }
        else {
            add_token(TokenType::SLASH);
        }
    } break;

    case '"': {
        tokenize_string();
    } break;

    case ' ':
    case '\r':
    case '\t':
        // Ignore general whitespaces
        break;

    case '\n':
        line += 1;
        break;

    default: {
        if (std::isdigit(next_ch)) {
            tokenize_number();
        }
        else if (std::isalpha(next_ch)) {
            tokenize_identifier();
        }
        else {
            // TODO(yemon): Do some sort of tokenization error reporting,
            // on an "invalid/unidentified" character.
        }
    } break;
    }
}

[[nodiscard]] char
Scanner::advance()
{
    try {
        return source.at(current++);
    }
    catch (const std::out_of_range &) {
        // TODO(yemon): Return some sort of empty or invalid result
        return '\0';
    }
}

[[nodiscard]] char 
Scanner::peek()
{
    if (is_end()) return '\0';
    try {
        return source.at(current);
    }
    catch (const std::out_of_range &) {
        // TODO(yemon): Return some sort of empty or invalid result
        return '\0';
    }
}

[[nodiscard]] char 
Scanner::peek_next()
{
    if (is_end() || current + 1 >= source.size()) {
        return '\0';
    }
    try {
        return source.at(current + 1);
    }
    catch (const std::out_of_range &) {
        // TODO(yemon): Return some sort of empty or invalid result
        return '\0';
    }
}

[[nodiscard]] bool 
Scanner::match_next(char expected) const
{
    if (is_end()) {
        return false;
    }
    if (source.at(current) != expected) {
        return false;
    }

    //current += 1;
    return true;
}

void
Scanner::add_token(const TokenType type)
{
    add_token(type, "");
}

// NOTE(yemon): I don't quite like the existance of this guy.
// Should probably think about simplifying this later on.
void 
Scanner::add_token(const TokenType type, const auto &literal)
{
    const auto lexeme = source.substr(start, current - start);
    Token new_token{ type, string{ lexeme }, string{ literal }, line};
    tokens.push_back(new_token);
}

void
Scanner::add_token(const TokenType type, const string_view lexeme, const string_view literal)
{
    Token new_token{ type, string{ lexeme }, string{ literal }, line };
    tokens.push_back(new_token);
}

void
Scanner::tokenize_string()
{
    while (peek() != '"' && !is_end()) {
        // Supporting the multi-line string literals
        if (peek() == '\n') {
            line += 1;
        }
        [[maybe_unused]] char _ = advance();
    }

    if (is_end()) {
        // TODO(yemon): Report invalid string composition error
        return;
    }

    // Mark the closing '"'
    [[maybe_unused]] char _ = advance();

    // Trim the start and end quotes
    size_t start_pos = start + 1;
    size_t count = (current - start_pos) - 1;
    string literal = source.substr(start_pos, count);
    add_token(TokenType::STRING, "", literal);
}

void
Scanner::tokenize_number()
{
    while (std::isdigit(peek())) {
        [[maybe_unused]] char _ = advance();
    }

    // Detect the fractional point '.'
    if (peek() == '.' && std::isdigit(peek_next())) {
        // Consume the '.'
        [[maybe_unused]] char _ = advance();

        // Consume the rest of the digits
        while (std::isdigit(peek())) {
            [[maybe_unused]] char _ = advance();
        }
    }

    // TODO(yemon): Only tokenizing the literal number value as string for now. 
    // Might want to specially parse it into their respective numeric types 
    // (double, int64, etc) later on.
    string number{ source.substr(start, current - start) };
    add_token(TokenType::NUMBER, "", number);
}

void 
Scanner::tokenize_identifier()
{
    char peek_ch = peek();
    while (is_alpha(peek_ch) || std::isdigit(peek_ch)) {
        [[maybe_unused]] char _ = advance();
        peek_ch = peek();
    }

    string identifier = source.substr(start, current - start);
    if (const auto search_it = KEYWORDS.find(string_view{ identifier }); 
        search_it != KEYWORDS.end()) {
        const TokenType token_type = search_it->second;
        add_token(token_type);
    }
    else {
        add_token(TokenType::IDENTIFIER, identifier, "");
    }
}

[[nodiscard]] bool
Scanner::is_end() const
{
    return current >= source.size();
}

[[nodiscard]] bool 
is_alpha(const char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
}

