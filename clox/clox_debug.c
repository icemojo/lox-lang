#include <stdio.h>
#include "clox_debug.h"

static uint32_t 
simple_instruction(const char *display, uint32_t offset)
{
    printf("%s\n", display);
    return offset + 1;
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

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
    case OP_RETURN: {
        return simple_instruction("OP_RETURN", offset);
    } break;

    default: {
        printf("(Unknown opcode %d\n)", instruction);
        return offset + 1;
    }
    }
}

