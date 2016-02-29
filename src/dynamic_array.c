#include "dynamic_array.h"


DynamicArray *array_create(size_t element_size, size_t initial_max) {
    DynamicArray *array = malloc(sizeof(DynamicArray));
    check_mem(array);

    array->max = initial_max;
    check(array->max > 0, "Intial max must be > 0");

    array->contents = calloc(initial_max, sizeof(void *));
    check_mem(array);

    array->end = 0;
    array->element_size = element_size;
    array->expand_rate = DEFAULT_EXPAND_RATE;

    return array;

    error:
    if (array) {
        free(array);
    }
    return NULL;
}

void array_clear(DynamicArray *array) {
    int i;

    if (array->element_size > 0) {
        for (i = 0; i < array->max; i++) {
            if (array->contents[i] != NULL) {
                free(array->contents[i]);
            }
        }
    }
}

static inline int array_resize(DynamicArray *array, size_t new_size) {
    array->max = new_size;
    check(array->max > 0, "The new size must be > 0");

    void *contents = realloc(array->contents, array->max * sizeof(void *));
    check_mem(contents);

    array->contents = contents;

    return 0;

    error:
    return -1;
}

int array_expand(DynamicArray *array) {
    size_t old_max = array->max;
    size_t new_size = array->max + array->expand_rate;

    check(array_resize(array, new_size) == 0, "Failed to expand array to new size: %zu", new_size);
    memset(array->contents + old_max, 0, array->expand_rate + 1);

    return 0;

    error:
    return -1;
}

int array_contract(DynamicArray *array) {
    size_t new_size = array->end < array->expand_rate ? array->expand_rate : (size_t) array->end;

    return array_resize(array, new_size + 1);
}

void array_destroy(DynamicArray *array) {
    if (array) {
        if (array->contents) {
            free(array->contents);
        }
        free(array);
    }
}

int array_push(DynamicArray *array, void *element) {
    array->contents[array->end] = element;
    array->end++;

    if (array->end >= array->max) {
        return array_expand(array);
    }
    return 0;
}

void *array_pop(DynamicArray *array) {
    check(array->end - 1 >= 0, "Can't pop from an empty array");

    void *element = array_remove(array, array->end - 1);
    array->end--;

    if (array->end > array->expand_rate && array->end % array->expand_rate) {
        array_contract(array);
    }
    return element;

    error:
    return NULL;
}
