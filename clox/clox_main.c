#include "clox_common.h"
#include "clox_chunk.h"
#include "clox_debug.h"

int main(const int argc, const char **argv)
{
    Chunk chunk;
    init_chunk(&chunk);

    uint32_t last_index = add_chunk_constant(&chunk, 3.14);
    write_chunk(&chunk, OP_CONSTANT, 1);
    write_chunk(&chunk, last_index, 1);

    write_chunk(&chunk, OP_RETURN, 2);

    disassemble_chunk(&chunk, "Test chunk");

    free_chunk(&chunk);

    return 0;
}
