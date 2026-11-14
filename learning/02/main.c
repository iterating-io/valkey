#include "array.h"

#include <stdio.h>

static void print_item(int value, size_t index, void *context) {
    (void)context;
    printf("data[%zu]=%d\n", index, value);
}

static void add_item(int value, size_t index, void *context) {
    int *sum = context;
    (void)index;
    *sum += value;
}

static void copy_ints(int *restrict destination, const int *restrict source,
                      size_t count) {
    size_t index;

    for (index = 0; index < count; index++) {
        destination[index] = source[index];
    }
}

int main(void) {
    IntArray array;
    int ok;
    int value;
    int sum = 0;
    int copied[5];
    const int source[5] = {0, 10, 20, 30, 40};

    printf("library=%s\n", array_library_name);
    if (!array_init(&array)) {
        fprintf(stderr, "failed to allocate array\n");
        return 1;
    }

    if (!array_push(&array, 0) || !array_push(&array, 10) ||
        !array_push(&array, 20) || !array_push(&array, 30) ||
        !array_push(&array, 40)) {
        fprintf(stderr, "failed to grow array\n");
        array_destroy(&array);
        return 1;
    }

    printf("length=%zu capacity=%zu\n", array.length, array.capacity);
    array_foreach(&array, print_item, NULL);
    array_foreach(&array, add_item, &sum);
    printf("sum=%d\n", sum);

    value = -1;
    ok = array_get(&array, 2, &value);
    printf("get(2): ok=%d value=%d\n", ok, value);
    ok = array_get(&array, 5, &value);
    printf("get(5): ok=%d value=%d\n", ok, value);

    copy_ints(copied, source, 5);
    printf("copied_last=%d\n", copied[4]);

    array_destroy(&array);
    return 0;
}
