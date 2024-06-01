#include <stdio.h>
#include "clox_vm.h"
#include "clox_debug.h"

static void reset_stack(VM *vm)
{
    vm->stack_top = vm->stack;
}

static Interpret_Result run(VM *vm)
{
#define READ_BYTE(vm)       (*vm->instruction_ptr++)
#define READ_CONSTANT(vm)   (vm->chunk->constants.values[READ_BYTE(vm)])
#define BINARY_OP(vm, op)   \
            do { \
                Value b = pop_stack(vm); \
                Value a = pop_stack(vm); \
                push_stack(vm, a op b); \
            } while (false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("== STACK: ");
        for (Value *slot = vm->stack; slot < vm->stack_top; slot += 1) {
            printf("[");
            print_value(*slot);
            printf("] ");
        }
        printf("==\n");

        uint32_t offset = (uint32_t)(vm->instruction_ptr - vm->chunk->codes);
        disassemble_instruction(vm->chunk, offset);
#endif
        uint8_t instruction = READ_BYTE(vm);
        switch (instruction) {
        case OP_CONSTANT: {
            Value constant = READ_CONSTANT(vm);
            push_stack(vm, constant);
        } break;

        case OP_ADD: {
            BINARY_OP(vm, +); 
        } break;

        case OP_SUBTRACT: {
            BINARY_OP(vm, -);
        } break;

        case OP_MULTIPLY: {
            BINARY_OP(vm, *);
        } break;

        case OP_DIVIDE: {
            BINARY_OP(vm, /);
        } break;

        case OP_NEGATE: {
            Value value = pop_stack(vm);
            push_stack(vm, -value);
        } break;

        case OP_RETURN: {
            print_value(pop_stack(vm));
            printf("\n");
            return INTERPRET_OK;    // TODO(yemon): Temp!
        } break;

        default:
            return INTERPRET_OK;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

//------------------------------------------------------------------------------

void init_vm(VM *vm)
{
    vm->chunk = NULL;
    vm->instruction_ptr = NULL;
    reset_stack(vm);
}

Interpret_Result interpret(VM *vm, Chunk *chunk)
{
    vm->chunk = chunk;
    vm->instruction_ptr = chunk->codes;

    return run(vm);
}

Interpret_Result interpret_line(VM *vm, const char *source_line)
{
    // TODO(yemon): ???
    return INTERPRET_OK;
}

void free_vm(VM *vm)
{
    vm->chunk = NULL;
    vm->instruction_ptr = NULL;
    init_vm(vm);
}

void push_stack(VM *vm, Value value)
{
    *(vm->stack_top) = value;
    vm->stack_top += 1;
}

Value pop_stack(VM *vm)
{
    vm->stack_top -= 1;
    return *(vm->stack_top);
}

