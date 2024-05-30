#include "clox_chunk.h"
#include "clox_memory.h"

void init_chunk(Chunk *chunk)
{
    chunk->count    = 0;
    chunk->capacity = 0;
    chunk->code     = NULL;
    chunk->lines    = NULL;
    init_value_array(&chunk->constants);
}

void free_chunk(Chunk *chunk)
{
    FREE_ARRAY(uint8_t,  chunk->code,  chunk->capacity);
    FREE_ARRAY(uint32_t, chunk->lines, chunk->capacity);
    free_value_array(&chunk->constants);
    init_chunk(chunk);
}

void write_chunk(Chunk *chunk, const uint8_t byte, const uint32_t line)
{
    if (chunk->capacity <= chunk->count) {
        int old_capacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old_capacity);
        chunk->code  = GROW_ARRAY(uint8_t,  chunk->code,  old_capacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(uint32_t, chunk->lines, old_capacity, chunk->capacity);
    }

    chunk->code [chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count += 1;
}

uint32_t add_chunk_constant(Chunk *chunk, const Value value)
{
    write_value_array(&chunk->constants, value);
    return chunk->constants.count - 1;
}
