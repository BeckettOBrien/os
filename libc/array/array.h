#pragma once
#include "types.h"

typedef struct {
    uint64_t elm_size;
    uint64_t length;
    uint64_t capacity;
    void* elements;
} array_t;

array_t array_alloc(uint64_t element_size);
void* array_get(array_t* array, uint64_t index);
int array_set(array_t* array, uint64_t index, void* value);
int array_push(array_t* array, void* value);
void* array_pop(array_t* array);
void array_free(array_t* array);