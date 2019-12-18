/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <misc/printk.h>
#include <zephyr.h>

K_SEM_DEFINE(light_sema, 0, 1);

enum { RED, GREEN, YELLOW, ALERT } state = YELLOW;
typedef enum { TIMEOUT, NONE, ALERT_EVT } event_t;

event_t current_event = NONE;

void expire_func(struct k_timer* timer)
{
    printk(" *** Timeout! ---------------------------------------\n");
    current_event = TIMEOUT;
    k_sem_give(&light_sema);
}

K_TIMER_DEFINE(light_timer, expire_func, NULL);
void state_machine_action()
{
    switch (state) {
    case RED:
        printk("-- RED State ----------\n");
        printk("RED on\n");
        printk("GREEN off\n");
        printk("YELLOW off\n");
        break;
    case GREEN:
        printk("-- GREEN State ----------\n");
        printk("RED off\n");
        printk("GREEN on\n");
        printk("YELLOW off\n");
        break;
    case YELLOW:
        printk("-- YELLOW State ----------\n");
        printk("RED off\n");
        printk("GREEN off\n");
        printk("YELLOW on\n");
        break;
    case ALERT:
        printk("-- ALERT State ----------\n");
        printk("YELLOW blink\n");
        break;
    default:
        break;
    }
}

void state_machine(event_t event)
{
    s32_t timeout = 0;
    switch (state) {
    case RED: {
        if (event == TIMEOUT) {
            state = GREEN;
        } else if (event == ALERT_EVT) {
            state = ALERT;
        }
        timeout = 3;
    } break;
    case GREEN: {
        if (event == TIMEOUT) {
            state = YELLOW;
        } else if (event == ALERT_EVT) {
            state = ALERT;
        }
        timeout = 1000;
    } break;
    case YELLOW: {
        if (event == TIMEOUT) {
            state = RED;
        } else if (event == ALERT_EVT) {
            state = ALERT;
        }
        timeout = 2000;
    } break;
    case ALERT: {
        if (event == ALERT_EVT) {
            state = RED;
        }
        timeout = 200;
    } break;
    default:
        state = RED;
    }
    state_machine_action();
    k_timer_start(&light_timer, K_MSEC(timeout), 0);
}

void alert_on()
{
    printk("Alert Toggle!!!");
    current_event = ALERT_EVT;
    k_timer_stop(&light_timer);
    k_sem_give(&light_sema);
}

void main()
{
    printk("Hello main thread!\n");
    state_machine(NONE);
    int count = 0;
    while (1) {
        k_sem_take(&light_sema, K_FOREVER);
        state_machine(current_event);
        printk("count %d\n", count);
        if (count == 5) {
            alert_on();
        }
        if (count == 20) {
            alert_on();
        }
        count++;
    }
}
