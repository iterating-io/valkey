#ifndef LEARNING_03_BITMAP_H
#define LEARNING_03_BITMAP_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *bytes;
    size_t byte_count;
} Bitmap;

/* Bit 0 is the least-significant bit of bytes[0]. */
int bitmap_setbit(Bitmap *bitmap, size_t bit_offset, int value);
int bitmap_getbit(const Bitmap *bitmap, size_t bit_offset, int *value);
size_t bitmap_countbit(const Bitmap *bitmap);

#endif
