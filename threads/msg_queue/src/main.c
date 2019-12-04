/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <gpio.h>
#include <misc/printk.h>
#include <zephyr.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

void main()
{
    printk("Hello world!\n");
    while (1) {
        k_sleep(K_MSEC(1000));
        printk(".\n");
    }
}

// K_THREAD_DEFINE(blink1_id, STACKSIZE, blink1, NULL, NULL, NULL, PRIORITY, 0,
// K_NO_WAIT); K_THREAD_DEFINE(blink2_id, STACKSIZE, blink2, NULL, NULL, NULL, PRIORITY,
// 0, K_NO_WAIT);