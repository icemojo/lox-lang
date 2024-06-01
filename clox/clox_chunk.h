#ifndef CLOX_CHUNK_H_
#define CLOX_CHUNK_H_

#include "clox_common.h"
#include "clox_value.h"

// NOTE(yemon): Instructions in each chunk can potentially have *different* sizes.
// For instance;
//     1 byte  =  OP_RETURN
//     2 bytes =  OP_CONSTANT [index]

typedef enum {
    OP_RETURN   = 0x00,
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
} Op_Code;

typedef struct {
    uint32_t     count;
    uint32_t     capacity;
    uint8_t      *codes;
    uint32_t     *lines;
    Value_Array  constants;
} Chunk;

void init_chunk(Chunk *chunk);

void free_chunk(Chunk *chunk);

void write_chunk(Chunk *chunk, const uint8_t byte, const uint32_t line);

uint32_t add_chunk_constant(Chunk *chunk, const Value value);

#endif // CLOX_CHUNK_H_
