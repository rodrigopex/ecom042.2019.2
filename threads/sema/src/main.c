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
#define PRIORITY 0

void main()
{
    printk("Hello main thread!\n");
    while (1) {
        k_sleep(K_SECONDS(1));
        printk(". main\n");
    }
}

u8_t counter = 0;
void thread2()
{
    printk("Hello thread 2!\n");
    while (1) {
        k_sleep(K_MSEC(300));
        printk(". thread 2: %d\n", counter);
        counter++;
        if (counter >= 10) {
            counter = 0;
        }
    }
}
void thread3()
{
    printk("Hello thread 3!\n");
    while (1) {
        if (counter == 9) {
            printk(". thread 3\n");
        } else {
            k_sleep(1);
        }
    }
}

K_THREAD_DEFINE(thread2_id, STACKSIZE, thread2, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(thread3_id, STACKSIZE, thread3, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);