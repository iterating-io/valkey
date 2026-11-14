#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct Layout {
    char flag;
    int count;
    char tail;
};

static void print_ints(int *values, size_t count) {
    size_t i;

    for (i = 0; i < count; i++) {
        printf("values[%zu] = %d, address = %p\n", i, values[i], (void *)&values[i]);
    }
}

int main(void) {
    int values[3] = {10, 20, 30};
    int *heap_value = malloc(sizeof(*heap_value));
    struct Layout layout;

    if (heap_value == NULL) {
        return 1;
    }

    *heap_value = 42;

    printf("sizeof(char)        = %zu\n", sizeof(char));
    printf("sizeof(int)         = %zu\n", sizeof(int));
    printf("sizeof(values)      = %zu\n", sizeof(values));
    printf("sizeof(values[0])   = %zu\n", sizeof(values[0]));
    printf("sizeof(struct Layout)= %zu\n", sizeof(layout));
    printf("sizeof(heap_value)  = %zu\n", sizeof(heap_value));
    printf("heap value          = %d\n", *heap_value);

    print_ints(values, sizeof(values) / sizeof(values[0]));

    /*
     * Answer these questions in learning/01/README.md or below before
     * looking up the explanation:
     *
     * 1. Why is sizeof(values) different from sizeof(heap_value)?
     * 2. Why does values + 1 advance by sizeof(int) bytes?
     * 3. Why can sizeof(struct Layout) be larger than the sum of its fields?
     * 4. Which expression gives the address of the second array element?
     * 5. Who owns heap_value here, and where must it be released?
     * 6. What would go wrong if print_ints received count == 4?
     */

    free(heap_value);
    return 0;
}
