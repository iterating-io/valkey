/*
 * Copyright (c) Valkey Contributors
 * All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>

int main(void) {
    int *number = malloc(sizeof(*number));

    if (number == NULL) return 1;
    *number = 42;
    free(number);
    return *number;
}
