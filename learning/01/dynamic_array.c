#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum { INITIAL_CAPACITY = 4 };

typedef struct {
    int *data;
    size_t length;
    size_t capacity;
} DynamicArray;

/* Implement this function as the next exercise. */
static int dynamic_array_get(const DynamicArray *array, size_t index, int *value) {
    if (index >= array->length) {
        return 0; 
    }
    *value = array->data[index];
    return 1;
}

static int dynamic_array_init(DynamicArray *array) {
    array->data = malloc(INITIAL_CAPACITY * sizeof(*array->data));
    if (array->data == NULL) {
        array->length = 0;
        array->capacity = 0;
        return 0;
    }

    array->length = 0;
    array->capacity = INITIAL_CAPACITY;
    return 1;
}

static int dynamic_array_push(DynamicArray *array, int value) {
    int *new_data;
    size_t new_capacity;

    if (array->length == array->capacity) {
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
    }

    array->data[array->length] = value;
    array->length++;
    return 1;
}

static void dynamic_array_destroy(DynamicArray *array) {
    free(array->data);
    array->data = NULL;
    array->length = 0;
    array->capacity = 0;
}

static void dynamic_array_print(const DynamicArray *array) {
    int value;
    size_t i;

    for (i = 0; i < array->length; i++) {
        if (!dynamic_array_get(array, i, &value)) {
            return;
        }
        printf("data[%zu]=%d\n", i, value);
    }
}

int main(void) {
    DynamicArray array;
    size_t i;

    if (!dynamic_array_init(&array)) {
        fprintf(stderr, "failed to allocate dynamic array\n");
        return 1;
    }

    for (i = 0; i < 10; i++) {
        if (!dynamic_array_push(&array, (int)(i * 10))) {
            fprintf(stderr, "failed to grow dynamic array\n");
            dynamic_array_destroy(&array);
            return 1;
        }
    }

    printf("length=%zu capacity=%zu\n", array.length, array.capacity);
    dynamic_array_print(&array);

    dynamic_array_destroy(&array);
    return 0;
}
