/*
 * Copyright (c) Valkey Contributors
 * All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

int main(void) {
    unsigned int shift = 32;
    uint32_t value = 1U << shift;

    return (int)value;
}
