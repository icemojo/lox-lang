#ifndef LOX_OPTIONS_
#define LOX_OPTIONS_

#include <string>
#include <vector>

using std::string;
using std::vector;

struct Options {
    bool tokenize_only = false;
    bool verbose = false;
    bool show_help = false;
    bool repl_start = false;

    string input_filename = "";
};

[[nodiscard]] Options
ParseOptions(int argc, char **argv);

[[nodiscard]] vector<string>
ReadInputFileLines(const string &filename, const bool append_line_breaks = false);

[[nodiscard]] string
ReadInputFileString(const string &filename);

#endif  // LOX_OPTIONS_
