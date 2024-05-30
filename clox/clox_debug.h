#ifndef CLOX_DEBUG_H_
#define CLOX_DEBUG_H_

#include "clox_chunk.h"

void disassemble_chunk(const Chunk *chunk, const char *name);

uint32_t disassemble_instruction(const Chunk *chunk, uint32_t offset);

#endif // CLOX_DEBUG_H_
