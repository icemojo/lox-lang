#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "clox_common.h"
#include "clox_vm.h"
#include "clox_chunk.h"
//#include "clox_debug.h"

typedef struct {
    bool verbose;
} Options;

static Options parse_options(const int argc, const char **argv)
{
    Options options;

    // TODO(yemon): Parse the command line options from the 3rd argument forward.

    return options;
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
    Input_File input_file = read_file(filepath);
    Interpret_Result result = interpret_line(vm, input_file.source);
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

        Interpret_Result result = interpret_line(vm, line_buffer);
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
    VM vm;
    init_vm(&vm);

    if (argc == 1) {
        start_repl(&vm);
    }
    else if (argc == 2) {
        run_file(&vm, argv[1]);
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
