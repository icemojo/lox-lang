#include <stdio.h>
#include "clox_debug.h"
#include "clox_value.h"

static uint32_t 
simple_instruction(const char *display, uint32_t offset)
{
    printf("%s\n", display);
    return offset + 1;
}

static uint32_t
constant_instruction(const char *display, const Chunk *chunk, uint32_t offset)
{
    uint8_t constant_index = chunk->code[offset+1];
    printf("%-12s [%d] '", display, constant_index);
    print_value(chunk->constants.values[constant_index]);
    printf("'\n");

    return offset + 2;
}

//------------------------------------------------------------------------------

void disassemble_chunk(const Chunk *chunk, const char *name)
{
    printf("== %s ==\n", name);

    for (uint32_t offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
}

uint32_t disassemble_instruction(const Chunk *chunk, uint32_t offset)
{
    printf("%04d ", offset);
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    }
    else {
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
    case OP_CONSTANT: {
        return constant_instruction("OP_CONSTANT", chunk, offset);
    } break;

    case OP_RETURN: {
        return simple_instruction("OP_RETURN", offset);
    } break;

    default: {
        printf("(Unknown opcode %d)\n", instruction);
        return offset + 1;
    }
    }
}

