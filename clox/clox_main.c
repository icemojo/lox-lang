#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "clox_common.h"
#include "clox_vm.h"
#include "clox_chunk.h"
//#include "clox_debug.h"

static bool str_cmp(const char *str1, const char *str2) 
{
    if (str1 == NULL || str2 == NULL) {
        return false;
    }
    return strcmp(str1, str2) == 0;
}

typedef struct {
    bool verbose;
    bool show_help;
    bool repl_start;
    const char *input_file;
} Options;

static Options parse_options(const int argc, const char **argv)
{
    Options options = {0};

    for (int i = 1; i < argc; i += 1) {
        const char *arg = argv[i];
        if (str_cmp(arg, "-v") || str_cmp(arg, "--verbose")) {
            options.verbose = true;
            continue;
        }
        if (str_cmp(arg, "-h") || str_cmp(arg, "--help")) {
            options.show_help = true;
            continue;
        }
        if (str_cmp(arg, "-r") || str_cmp(arg, "--repl")) {
            options.repl_start = true; 
            continue;
        }
        
        options.input_file = arg;
    }

    return options;
}

static void print_options(const Options *options)
{
    printf("Options: {\n");
    printf("\tverbose=%s, \n", options->verbose ? "true" : "false");
    printf("\tshow_help=%s, \n", options->show_help ? "true" : "false");
    printf("\trepl_start=%s, \n", options->repl_start ? "true" : "false");
    printf("\tinput_file=\"%s\", \n", options->input_file != NULL ? options->input_file : "");
    printf("}\n");
}

typedef struct {
    size_t file_size;
    char *source;
} Input_File;

static Input_File read_file(const char *filepath)
{
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Unable to open the input file \"%s\".\n", filepath);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read the file contents \"%s\".\n", filepath);
        exit(74);
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Unable to read the file contents \"%s\".\n", filepath);
        exit(74);
    }

    buffer[bytes_read] = '\0';

    fclose(file);
    Input_File input = { .file_size = file_size, .source = buffer };
    return input;
}

static void run_file(VM *vm, const char *filepath)
{
    // TODO(yemon): Technically, the `Input_File` is not limited to the `run_file(..)`
    // repl inputs can be parsed as in Input_File and pass along to the Lexer as well. 
    Input_File input_file = read_file(filepath);
    Interpret_Result result = interpret_source(vm, input_file.source);
    free(input_file.source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

static void start_repl(VM *vm)
{
    char line_buffer[1024];
    for (;;) {
        printf(">> ");

        if (!fgets(line_buffer, sizeof(line_buffer), stdin)) {
            printf("\n");
            break;
        }

        //printf("%s", line_buffer);
        Interpret_Result result = interpret_source(vm, line_buffer);
    }
}

static void write_test_program(Chunk *chunk)
{
    uint32_t last_index = add_chunk_constant(chunk, 3.14);
    write_chunk(chunk, OP_CONSTANT, 1);
    write_chunk(chunk, last_index, 1);
    write_chunk(chunk, OP_NEGATE, 1);              // -3.14

    last_index = add_chunk_constant(chunk, 12);    // 12+20
    write_chunk(chunk, OP_CONSTANT, 2);
    write_chunk(chunk, last_index, 2);
    last_index = add_chunk_constant(chunk, 20);
    write_chunk(chunk, OP_CONSTANT, 2);
    write_chunk(chunk, last_index, 2);
    write_chunk(chunk, OP_ADD, 2);                 // =32

    write_chunk(chunk, OP_RETURN, 3);              // return 32
}

int main(const int argc, const char **argv)
{
    Options options = parse_options(argc, argv);
    print_options(&options);

    VM vm;
    init_vm(&vm);

    if (options.repl_start) {
        start_repl(&vm);
    }
    else if (options.input_file != NULL) {
        run_file(&vm, options.input_file);
    }
    else {
        // TODO(yemon): Parse the rest of the command line options
        fprintf(stderr, "Usage: clox [filepath]\n");
        exit(64);
    }
    
    //Chunk chunk;
    //init_chunk(&chunk);

    //write_test_program(&chunk);
    //disassemble_chunk(&chunk, "Test chunk");

    //Interpret_Result result = interpret(&vm, &chunk);

    //free_chunk(&chunk);
    free_vm(&vm);

    return 0;
}
