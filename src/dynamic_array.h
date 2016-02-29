#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>
#include <assert.h>
#include <lcthw/debugger.h>

#define DEFAULT_EXPAND_RATE 50

typedef struct {
    int end;
    size_t max;
    size_t element_size;
    size_t expand_rate;
    void **contents;
} DynamicArray;

DynamicArray *array_create(size_t element_size, size_t initial_max);

void array_destroy(DynamicArray *array);

void array_clear(DynamicArray *array);

int array_expand(DynamicArray *array);

int array_contract(DynamicArray *array);

int array_push(DynamicArray *array, void *element);

void *array_pop(DynamicArray *array);


static inline void array_set(DynamicArray *array, int position, void *element) {
    check(position < array->max, "Position can't be over array max");

    if(position > array->end){
        array->end = position;
    }
    array->contents[position] = element;

    error:
    return;
}

static inline void *array_get(DynamicArray *array, int position){
    check(position < array->max, "Position can't be over array max");

    return array->contents[position];

    error:
    return NULL;
}

static inline void *array_remove(DynamicArray *array, int position){
    void *element = array->contents[position];

    array->contents[position] = NULL;

    return element;
}

static inline void *array_new(DynamicArray *array){
    check(array->element_size > 0, "Array size is 0");

    return calloc(1, array->element_size);

    error:
    return NULL;
}

#endif
