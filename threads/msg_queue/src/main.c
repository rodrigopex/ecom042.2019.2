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

K_MSGQ_DEFINE(pipe, sizeof(u32_t), 10, 4);

struct data_t {
    u16_t lo;
    u16_t hi;
    u16_t average;
};

K_MSGQ_DEFINE(data_pipe, sizeof(struct data_t), 2, 2);

void main()
{
    printk("Main!\n");
    printk("Size of struct data = %d\n", sizeof(struct data_t));
    u32_t counter = 0;
    while (!k_msgq_put(&pipe, &counter, K_FOREVER)) {
        printk(". main put %d\n", counter);
        counter++;
        k_busy_wait(500000);
    }
}
void t2()
{
    printk("t2!\n");
    u32_t data = 0;
    while (1) {
        k_msgq_get(&pipe, &data, K_FOREVER);
        struct data_t new_data = {
            .lo = data, .hi = data >> 16, .average = (new_data.hi + new_data.lo) >> 1};
        k_msgq_put(&data_pipe, &new_data, K_FOREVER);
        printk(". t2 -> %d\n", data);
    }
}

void t3()
{
    printk("t3!\n");
    u32_t data = 0;
    while (1) {
        struct data_t new_data = {0};
        k_msgq_get(&data_pipe, &new_data, K_FOREVER);
        printk(". t3 -> hi = %d, lo = %d, av = %d\n", new_data.hi, new_data.lo,
               new_data.average);
    }
}

void t4()
{
    printk("t4!\n");
    u32_t data = 0;
    while (1) {
        struct data_t new_data = {0};
        k_msgq_get(&data_pipe, &new_data, K_FOREVER);
        printk(". t4 -> hi = %d, lo = %d, av = %d\n", new_data.hi, new_data.lo,
               new_data.average);
    }
}


K_THREAD_DEFINE(t2_id, STACKSIZE, t2, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(t3_id, STACKSIZE, t3, NULL, NULL, NULL, PRIORITY - 1, 0, K_NO_WAIT);
K_THREAD_DEFINE(t4_id, STACKSIZE, t4, NULL, NULL, NULL, PRIORITY - 2, 0, K_NO_WAIT);
