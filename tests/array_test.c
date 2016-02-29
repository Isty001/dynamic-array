#include <minunit.h>
#include "../src/dynamic_array.h"


static DynamicArray *array = NULL;
static int *value0 = NULL;
static int *value1 = NULL;

void test_create() {
    array = array_create(sizeof(int), 100);

    mu_assert(array != NULL, "Array create failed");
    mu_assert(array->contents != NULL, "Contents are wrongly set in array");
    mu_assert(array->end == 0, "End is at the wrong position");
    mu_assert(array->element_size == sizeof(int), "Element size is wrong");
    mu_assert(array->max == 100, "Array max is wrongly set");
}

void test_new() {
    value0 = array_new(array);
    mu_assert(value0 != NULL, "Failed to make a new element");

    value1 = array_new(array);
    mu_assert(value1 != NULL, "Failed to make a new element");
}

void test_set() {
    array_set(array, 0, value0);
    array_set(array, 1, value1);
}

void test_get() {
    mu_assert(array_get(array, 0) == value0, "Shoulbe value0");
}

void test_remove() {
    int *val_check = array_remove(array, 0);

    mu_assert(val_check != NULL, "Value check shouldn't be NULL");
    mu_assert(*val_check == *value0, "Should be value0");
    mu_assert(array_get(array, 0) == NULL, "Value0 should be NULL");
    free(val_check);
}

void test_expand_contract() {
    size_t old_max = array->max;

    array_expand(array);
    mu_assert((unsigned int) array->max == old_max + array->expand_rate, "Wrong size after expand");

    array_contract(array);
    mu_assert((unsigned int) array->max == array->expand_rate + 1, "Should stay at the expand rate at least");
}

void test_push_pop() {
    int i;

    for (i = 0; i < 100; i++) {
        int *element = array_new(array);
        *element = i * 333;
        array_push(array, element);
    }
    mu_assert(array->max == 151, "Wrong max size");

    for (i = 99; i >= 0; i--) {
        int *value = array_pop(array);

        mu_assert(value != NULL, "Shouldn't get NULL");
        mu_assert(*value == i * 333, "Wrong value");
        free(value);
    }
}

void test_destroy() {
    array_destroy(array);
}

int main(int argc, char *argv[]) {

    MU_RUN_TEST(test_create);
    MU_RUN_TEST(test_new);
    MU_RUN_TEST(test_set);
    MU_RUN_TEST(test_get);
    MU_RUN_TEST(test_remove);
    MU_RUN_TEST(test_expand_contract);
    MU_RUN_TEST(test_push_pop);
    MU_RUN_TEST(test_destroy);

    MU_REPORT();

    return 0;
}
