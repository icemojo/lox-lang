#include "clox_common.h"
#include "clox_vm.h"
#include "clox_chunk.h"
#include "clox_debug.h"

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

    Chunk chunk;
    init_chunk(&chunk);

    write_test_program(&chunk);
    //disassemble_chunk(&chunk, "Test chunk");

    Interpret_Result result = interpret(&vm, &chunk);

    free_chunk(&chunk);
    free_vm(&vm);

    return 0;
}
