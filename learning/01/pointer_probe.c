#include <stddef.h>
#include <stdio.h>

static void inspect_array(int *values, size_t count) {
    size_t i;

    printf("inside function: sizeof(values) = %zu\n", sizeof(values));
    for (i = 0; i < count; i++) {
        printf("values[%zu] = %d, address = %p\n", i, values[i], (void *)&values[i]);
    }
}

int main(void) {
    int values[3] = {10, 20, 30};
    int *p = values;

    printf("array: sizeof(values) = %zu\n", sizeof(values));
    printf("element: sizeof(values[0]) = %zu\n", sizeof(values[0]));
    printf("p points to values[0]: %d\n", *p);
    printf("p + 1 points to values[1]: %d\n", *(p + 1));
    printf("&values[2] == values + 2: %d\n", &values[2] == values + 2);

    inspect_array(values, sizeof(values) / sizeof(values[0]));
    return 0;
}
