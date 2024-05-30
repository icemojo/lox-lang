#ifndef CLOX_VALUE_H_
#define CLOX_VALUE_H_

#include "clox_common.h"

typedef double Value;

void print_value(const Value value);

// NOTE(yemon): The "constant-data pool" will hold both the *immediate* values and
// the *actual constant* values (such as literal strings) as well.
typedef struct {
    uint32_t count;
    uint32_t capacity;
    Value *values;
} Value_Array;

void init_value_array(Value_Array *array);

void write_value_array(Value_Array *array, const Value value);

void free_value_array(Value_Array *array);

bool empty_value_array(Value_Array *array);

#endif // CLOX_VALUE_H_
