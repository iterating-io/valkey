/*
 * Copyright (c) Valkey Contributors
 * All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pthread.h>

static int counter;

static void *increment_counter(void *argument) {
    int index;

    (void)argument;
    for (index = 0; index < 100000; index++) {
        counter++;
    }
    return NULL;
}

int main(void) {
    pthread_t first;
    pthread_t second;

    if (pthread_create(&first, NULL, increment_counter, NULL) != 0) return 1;
    if (pthread_create(&second, NULL, increment_counter, NULL) != 0) return 1;
    if (pthread_join(first, NULL) != 0) return 1;
    if (pthread_join(second, NULL) != 0) return 1;
    return counter == 200000 ? 0 : 1;
}
