#include <stdio.h>
#include "clox_common.h"
#include "clox_compiler.h"
#include "clox_lexer.h"

void compile(const char *source_line)
{
    Scanner scanner = {0};
    init_scanner(&scanner, source_line);

    // TODO(yemon): Temp to kick off the scanner pipeline for the time being!
    int32_t line = -1;
    for (;;) {
        Token new_token = scan_token(&scanner);
        if (new_token.line != line) {
            printf("%4d ", new_token.line);
            new_token.line = line;
        }
        else {
            printf("   | ");
        }
        printf("%16s '%.*s'\n", token_type_to_string(new_token.type), new_token.length, new_token.start);

        if (new_token.type == TOKEN_EOF) {
            break;
        }
        if (new_token.type == TOKEN_ERROR) {
            break;
        }
    }
}

