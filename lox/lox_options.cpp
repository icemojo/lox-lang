#include <iostream>
#include <format>
#include <ranges>
#include <fstream>
#include "lox_options.h"


[[nodiscard]] Options
ParseOptions(int argc, char **argv)
{
    Options options{};
    if (argc == 1) {
        options.repl_start = true;
        return options;
    }

    auto to_string = [](const char *v) { return string(v); };
    auto args = std::span(argv, argc) | std::views::transform(to_string);

    for (auto it = args.begin() + 1; it != args.end(); it += 1) {
        if (*it == "-t" || *it == "--token") {
            options.tokenize_only = true;
            continue;
        }
        if (*it == "-v" || *it == "--verbose") {
            options.verbose = true;
            continue;
        }
        if (*it == "-h" || *it == "--help") {
            options.verbose = true;
            continue;
        }

        options.input_filename = *it;
    }

    return options;
}

[[nodiscard]] vector<string>
ReadInputFileLines(const string &filename, const bool append_line_breaks)
{
    vector<string> lines{};
    std::ifstream input(filename);

    if (input.fail()) {
        std::cout << std::format("ERROR: Unable to open input file \"{}\".\n", filename);
        input.close();
        lines.clear();
        return lines;
    }

    string line_buffer;
    while (std::getline(input, line_buffer)) {
        if (append_line_breaks) {
            line_buffer.push_back('\n');
        }
        lines.push_back(line_buffer);
    }

    input.close();
    return lines;
}

[[nodiscard]] string
ReadInputFileString(const string &filename)
{
    std::ifstream input(filename);
    string result = "";

    if (input.fail()) {
        std::cout << std::format("ERROR: Unable to open input file \"{}\".\n", filename);
        input.close();
        result.clear();
        return result;
    }

    std::getline(input, result, '\0');
    return result;
}

