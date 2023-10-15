#include "array.h"
#include "kernel/memory/kmalloc.h"
#include "memory/memory.h"

#define ARRAY_SIZE_INCREMENT 5

// realloc whole array?

array_t array_alloc(uint64_t element_size) {
    array_t array;
    array.elm_size = element_size;
    array.length = 0;
    array.capacity = ARRAY_SIZE_INCREMENT;
    array.elements = kmalloc(element_size * ARRAY_SIZE_INCREMENT);
    return array;
}

void* array_get(array_t* array, uint64_t index) {
    if (index >= array->elements) return NULL;
    return array->elements + (index * array->elm_size);
}

int array_set(array_t* array, uint64_t index, void* value) {
    if (index >= array->capacity) {
        array->capacity += ARRAY_SIZE_INCREMENT;
        //TODO: Check if capacity has overflowed
        array->elements = realloc(array->elements, array->capacity);
        if (array->elements == NULL) return -1;
    }
    memcpy(array->elements + (index * array->elm_size), value, array->elm_size);
    if (array->length < index + 1) array->length = index + 1;
    return 1;
}

inline int array_push(array_t* array, void* value) {
    return array_set(array, array->length++, value);
}

void* array_pop(array_t* array) {
    void* val = array_get(array, array->length--);
    if (array->length <= (array->capacity - ARRAY_SIZE_INCREMENT)) {
        // Shrink array if possible
        array->capacity -= ARRAY_SIZE_INCREMENT;
        array->elements = realloc(array->elements, array->capacity);
    }
    return val;
}

void array_free(array_t* array) {
    kfree(array->elements);
    array->elements = NULL;
    array->length = 0;
    array->capacity = 0;
}