#include "bitmap.h"

#include <limits.h>

static int bitmap_bit_location(const Bitmap *bitmap, size_t bit_offset,
                               size_t *byte_index, uint8_t *mask) {
    size_t index;

    index = bit_offset / CHAR_BIT;
    if (index >= bitmap->byte_count) {
        return 0;
    }

    *byte_index = index;
    *mask = (uint8_t)(UINT8_C(1) << (bit_offset % CHAR_BIT));
    return 1;
}

int bitmap_setbit(Bitmap *bitmap, size_t bit_offset, int value) {
    size_t byte_index;
    uint8_t mask;

    if (!bitmap_bit_location(bitmap, bit_offset, &byte_index, &mask)) {
        return 0;
    }

    if (value != 0) {
        bitmap->bytes[byte_index] |= mask;
    } else {
        bitmap->bytes[byte_index] &= (uint8_t)~mask;
    }
    return 1;
}

int bitmap_getbit(const Bitmap *bitmap, size_t bit_offset, int *value) {
    size_t byte_index;
    uint8_t mask;

    if (!bitmap_bit_location(bitmap, bit_offset, &byte_index, &mask)) {
        return 0;
    }

    *value = (bitmap->bytes[byte_index] & mask) != 0;
    return 1;
}

size_t bitmap_countbit(const Bitmap *bitmap) {
    size_t byte_index;
    size_t count = 0;

    for (byte_index = 0; byte_index < bitmap->byte_count; byte_index++) {
        uint8_t byte = bitmap->bytes[byte_index];

        while (byte != 0) {
            count += byte & UINT8_C(1);
            byte >>= 1;
        }
    }
    return count;
}
