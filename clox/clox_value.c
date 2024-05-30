#include <stdio.h>
#include "clox_memory.h"
#include "clox_value.h"

void print_value(const Value value)
{
    printf("%g", value);
}

void init_value_array(Value_Array *array)
{
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void write_value_array(Value_Array *array, const Value value)
{
    if (array->capacity <= array->count) {
        uint32_t old_capacity = array->capacity;
        array->capacity = GROW_CAPACITY(old_capacity);
        array->values = GROW_ARRAY(Value, array->values, old_capacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count += 1;
}

void free_value_array(Value_Array *array)
{
    FREE_ARRAY(Value, array->values, array->capacity);
    init_value_array(array);
}

bool empty_value_array(Value_Array *array)
{
    if (array == NULL) {
        return true;
    }
    if (array->count == 0 || array->values == NULL) {
        return true;
    }
    return false;
}

