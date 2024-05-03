#include <iostream>
#include <format>
#include "lox_options.h"
#include "lox_lexer.h"
#include "lox_ast.h"


static const std::string_view PROMPT = ">> ";

inline bool has_error   = false;
inline bool should_quit = false;

//------------------------------------------------------------------------------

static void
Run(const std::string_view &source, const Options &options);

static void 
ReplStart(const Options &options)
{
    std::cout << "Lox Language Interpreter:\n";
    std::cout << "(Type 'lox --help' to print out available options)\n";
    if (options.verbose) {
        std::cout << "(Verbose mode turned on)\n";
    }

    std::string input_buffer{};

    while (!should_quit) {
        std::cout << PROMPT;
        std::getline(std::cin, input_buffer);

        if (std::cin.fail()) {
            std::cin.clear();
            should_quit = true;
            break;
        }
        if (input_buffer.empty()) {
            continue;
        }

        Run(input_buffer, options);

        has_error = false;
        input_buffer.clear();
    }
}

static void
RunFile(const Options &options)
{
    if (options.input_filename.size() == 0) {
        return;
    }

    Run(options.input_filename, options);
}

static void
Run(const std::string_view &source, const Options &options)
{
    // TODO(yemon): initialize lexer-parser toolchain with the input text
    Scanner scanner{ source };
    scanner.scan_tokens();

    for (const auto &token : scanner.get_tokens()) {
        std::cout << token.to_string() << '\n';
    }
}

static void
ReportError(const int32_t line, const std::string_view &where, const std::string_view &message)
{
    std::cout << std::format("[{}] ERROR {}: {}\n", line, where, message);
    has_error = true;
}

//------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    //test2();
    //return 0;

    Options options = ParseOptions(argc, argv);

    if (options.repl_start) {
        ReplStart(options);
    }
    else {
        RunFile(options);
    }

    if (should_quit || !has_error) {
        return 0;
    }
    else {
        // TODO(yemon): Define some error codes to report back to the OS
        return -1;
    }
}
