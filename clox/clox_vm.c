#include <stdio.h>
#include "clox_vm.h"
#include "clox_debug.h"

static Interpret_Result run(VM *vm)
{
#define READ_BYTE(vm)       (*vm->instruction_ptr++)
#define READ_CONSTANT(vm)   (vm->chunk->constants.values[READ_BYTE(vm)])

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        uint32_t offset = (uint32_t)(vm->instruction_ptr - vm->chunk->codes);
        disassemble_instruction(vm->chunk, offset);
#endif
        uint8_t instruction = READ_BYTE(vm);
        switch (instruction) {
        case OP_CONSTANT: {
            Value constant = READ_CONSTANT(vm);
            print_value(constant);
            printf("\n");
        } break;

        case OP_RETURN: {
            return INTERPRET_OK;    // TODO(yemon): Temp!
        } break;

        default:
            return INTERPRET_OK;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
}

//------------------------------------------------------------------------------

void init_vm(VM *vm)
{
    vm->chunk = NULL;
    vm->instruction_ptr = NULL;
}

Interpret_Result interpret(VM *vm, Chunk *chunk)
{
    vm->chunk = chunk;
    vm->instruction_ptr = chunk->codes;

    return run(vm);
}

void free_vm(VM *vm)
{
    vm->chunk = NULL;
    vm->instruction_ptr = NULL;
    init_vm(vm);
}

