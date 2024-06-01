#ifndef CLOX_VM_H_
#define CLOX_VM_H_

#include "clox_common.h"
#include "clox_chunk.h"
#include "clox_value.h"

#define STACK_MAX 256

typedef struct {
    Chunk *chunk;
    uint8_t *instruction_ptr;   // the name 'ip' is standard in the compiler world
    Value stack[STACK_MAX];
    Value *stack_top;
} VM;

typedef enum {
    INTERPRET_OK = 0x01,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} Interpret_Result;

void init_vm(VM *vm);

Interpret_Result interpret(VM *vm, Chunk *chunk);

void free_vm(VM *vm);

void push_stack(VM *vm, Value value);

Value pop_stack(VM *vm);

#endif // CLOX_VM_H_
