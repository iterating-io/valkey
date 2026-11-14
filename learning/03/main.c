#include "bitmap.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>

static void print_bytes(const void *address, size_t length) {
    const uint8_t *bytes = address;
    size_t index;

    for (index = 0; index < length; index++) {
        printf(" %02x", bytes[index]);
    }
    printf("\n");
}

static int require(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "check failed: %s\n", message);
        return 0;
    }
    return 1;
}

int main(void) {
    uint8_t storage[2] = {0, 0};
    Bitmap bitmap = {storage, sizeof(storage)};
    uint32_t word = UINT32_C(0x01020304);
    uint8_t unsigned_max = UINT8_MAX;
    atomic_uint counter = ATOMIC_VAR_INIT(0);
    int bit;

    if (!require(bitmap_setbit(&bitmap, 0, 1), "set bit 0") ||
        !require(bitmap_setbit(&bitmap, 7, 1), "set bit 7") ||
        !require(bitmap_setbit(&bitmap, 8, 1), "set bit 8")) {
        return 1;
    }

    printf("bitmap bytes:");
    print_bytes(storage, sizeof(storage));
    printf("count=%zu\n", bitmap_countbit(&bitmap));
    printf("word 0x%08x stored as:", word);
    print_bytes(&word, sizeof(word));
    printf("addresses: storage=%p storage[1]=%p distance=%td\n", (void *)&storage[0],
           (void *)&storage[1], &storage[1] - &storage[0]);
    printf("uint8: %u + 1 = %u\n", (unsigned int)unsigned_max,
           (unsigned int)(uint8_t)(unsigned_max + 1));

    atomic_fetch_add_explicit(&counter, 1, memory_order_relaxed);
    atomic_fetch_add_explicit(&counter, 1, memory_order_relaxed);
    printf("atomic counter=%u\n", atomic_load_explicit(&counter, memory_order_relaxed));
    return 0;
}
