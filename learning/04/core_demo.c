/*
 * Copyright (c) Valkey Contributors
 * All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>

static void report_fatal_error(void) {
    abort();
}

int main(void) {
    report_fatal_error();
    return 0;
}
