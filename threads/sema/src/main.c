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


K_SEM_DEFINE(t3_sema, 0, 1);
K_SEM_DEFINE(var_sema, 1, 1);
u8_t var = 0;

void main()
{
    printk("Hello main thread!\n");
    while (1) {
        k_sleep(K_SECONDS(1));
        printk(". main %d\n", var);
    }
}

void thread2()
{
    u8_t counter = 0;
    printk("Hello thread 2!\n");
    while (1) {
        k_sleep(K_MSEC(300));
        printk(". thread 2: %d\n", counter);
        counter++;
        k_sem_take(&var_sema, K_FOREVER);
        var = 2;
        if (var == 2) {
            var = 0;
        }
        k_sem_give(&var_sema);
        if (counter >= 10) {
            counter = 0;
            k_sem_give(&t3_sema);
        }
    }
}

void thread3()
{
    printk("Hello thread 3!\n");
    while (1) {
        if (k_sem_take(&t3_sema, K_MSEC(100))) {
            k_sem_take(&var_sema, K_FOREVER);
            var = 3;
            k_sem_give(&var_sema);
        } else {
            printk(". --------- thread 3\n");
        }
    }
}

K_THREAD_DEFINE(thread2_id, STACKSIZE, thread2, NULL, NULL, NULL, PRIORITY + 1, 0,
                K_NO_WAIT);
K_THREAD_DEFINE(thread3_id, STACKSIZE, thread3, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);