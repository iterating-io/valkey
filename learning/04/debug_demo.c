/*
 * Copyright (c) Valkey Contributors
 * All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

static int apply_coupon(int subtotal, int coupon) {
    int total = subtotal - coupon;

    return total;
}

static int calculate_total(int unit_price, int quantity) {
    int subtotal = unit_price * quantity;
    int coupon = quantity >= 3 ? 5 : 0;

    return apply_coupon(subtotal, coupon);
}

int main(void) {
    int unit_price = 10;
    int quantity = 3;
    int total = calculate_total(unit_price, quantity);

    /* Intentional logic bug: this should compare total with 25, not 30. */
    if (total == 30) {
        printf("total is correct: %d\n", total);
    } else {
        printf("unexpected total: %d\n", total);
    }
    return 0;
}
