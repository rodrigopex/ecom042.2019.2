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

K_MSGQ_DEFINE(pipe, 4, 10, 4);

void main()
{
    printk("Main!\n");
    u32_t counter = 0;
    while (1) {
        k_sleep(K_MSEC(1000));
        printk(". main\n");
        counter++;
        while (!k_msgq_put(&pipe, &counter, K_SECONDS(1))) {
        }
    }
}
void t2()
{
    printk("t2!\n");
    u32_t data = 0;
    while (1) {
        k_msgq_get(&pipe, &data, K_FOREVER);
        printk(". %d\n", data);
    }
}

K_THREAD_DEFINE(t2_id, STACKSIZE, t2, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
// K_THREAD_DEFINE(blink2_id, STACKSIZE, blink2, NULL, NULL, NULL, PRIORITY, 0,
// K_NO_WAIT);