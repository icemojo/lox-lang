#ifndef CLOX_CHUNK_H_
#define CLOX_CHUNK_H_

#include "clox_common.h"

// NOTE(yemon): Instructions can potentially have *different* sizes.

typedef enum {
    OP_RETURN,
} Op_Code;

typedef struct {
    uint32_t count;
    uint32_t capacity;
    uint8_t *code;
} Chunk;

void init_chunk(Chunk *chunk);

void free_chunk(Chunk *chunk);

void write_chunk(Chunk *chunk, uint8_t byte);

#endif // CLOX_CHUNK_H_
