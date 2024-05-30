#include "clox_common.h"
#include "clox_chunk.h"
#include "clox_debug.h"

int main(const int argc, const char **argv)
{
    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, OP_RETURN);
    disassemble_chunk(&chunk, "Test chunk");
    free_chunk(&chunk);

    return 0;
}
