#ifndef LEARNING_02_ARRAY_H
#define LEARNING_02_ARRAY_H

#include <stddef.h>

#define ARRAY_INITIAL_CAPACITY 4

typedef struct {
    int *data;
    size_t length;
    size_t capacity;
} IntArray;

/* Defined in array.c; the declaration is shared with users through this header. */
extern const char array_library_name[];

int array_init(IntArray *array);
int array_push(IntArray *array, int value);
int array_get(const IntArray *array, size_t index, int *value);
void array_foreach(const IntArray *array,
                   void (*callback)(int value, size_t index, void *context),
                   void *context);
void array_destroy(IntArray *array);

#endif
