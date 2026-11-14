#include "array.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const char array_library_name[] = "learning-02-array";

static int array_grow(IntArray *array) {
    int *new_data;
    size_t new_capacity;

    if (array->capacity > SIZE_MAX / 2 ||
        array->capacity * 2 > SIZE_MAX / sizeof(*array->data)) {
        return 0;
    }

    new_capacity = array->capacity * 2;
    new_data = realloc(array->data, new_capacity * sizeof(*array->data));
    if (new_data == NULL) {
        return 0;
    }

    array->data = new_data;
    array->capacity = new_capacity;
    return 1;
}

int array_init(IntArray *array) {
    array->data = malloc(ARRAY_INITIAL_CAPACITY * sizeof(*array->data));
    if (array->data == NULL) {
        array->length = 0;
        array->capacity = 0;
        return 0;
    }

    array->length = 0;
    array->capacity = ARRAY_INITIAL_CAPACITY;
    return 1;
}

int array_push(IntArray *array, int value) {
    if (array->length == array->capacity && !array_grow(array)) {
        return 0;
    }

    array->data[array->length] = value;
    array->length++;

#ifdef ARRAY_TRACE
    fprintf(stderr, "array_push: length=%zu capacity=%zu\n", array->length,
            array->capacity);
#endif
    return 1;
}

int array_get(const IntArray *array, size_t index, int *value) {
    if (index >= array->length) {
        return 0;
    }

    *value = array->data[index];
    return 1;
}

void array_foreach(const IntArray *array,
                   void (*callback)(int value, size_t index, void *context),
                   void *context) {
    size_t index;

    for (index = 0; index < array->length; index++) {
        callback(array->data[index], index, context);
    }
}

void array_destroy(IntArray *array) {
    free(array->data);
    array->data = NULL;
    array->length = 0;
    array->capacity = 0;
}
