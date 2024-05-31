#include "clox_common.h"
#include "clox_vm.h"
#include "clox_chunk.h"
#include "clox_debug.h"

int main(const int argc, const char **argv)
{
    VM vm;
    init_vm(&vm);

    Chunk chunk;
    init_chunk(&chunk);

    uint32_t last_index = add_chunk_constant(&chunk, 3.14);
    write_chunk(&chunk, OP_CONSTANT, 1);
    write_chunk(&chunk, last_index, 1);

    write_chunk(&chunk, OP_RETURN, 2);

    disassemble_chunk(&chunk, "Test chunk");

    Interpret_Result result = interpret(&vm, &chunk);

    free_chunk(&chunk);
    free_vm(&vm);

    return 0;
}
