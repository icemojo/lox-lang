#include "clox_common.h"
#include "clox_chunk.h"

int main(const int argc, const char **argv)
{
    Chunk chunk;
    init_chunk(&chunk);
    write_chunk(&chunk, OP_RETURN);
    free_chunk(&chunk);

    return 0;
}
