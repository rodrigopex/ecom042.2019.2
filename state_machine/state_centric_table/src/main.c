/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <misc/printk.h>
#include <zephyr.h>

K_SEM_DEFINE(light_sema, 1, 1);

typedef enum { RED, GREEN, YELLOW, ALERT, NUMBER_OF_STATES } state_t;
state_t current_state = YELLOW;

typedef enum { TIMEOUT_EVENT, ALERT_EVENT, NUMBER_OF_EVENTS } event_t;

struct state_machine_table_row {
    state_t next_state[NUMBER_OF_EVENTS];
    void (*enter_action)();
    void (*exit_action)();
    s32_t timeout;
};

typedef struct state_machine_table_row state_machine_table_row_t;

void red_enter()
{
    printk("-- RED State ENTER ----------\n");
    printk("RED on\n");
}

void red_exit()
{
    printk("-- RED State EXIT ----------\n");
    printk("RED off\n");
}

void green_enter()
{
    printk("-- GREEN State ENTER ----------\n");
    printk("GREEN on\n");
}

void green_exit()
{
    printk("-- GREEN State EXIT ----------\n");
    printk("GREEN off\n");
}

void yellow_enter()
{
    printk("-- YELLOW State ENTER ----------\n");
    printk("YELLOW on\n");
}

void yellow_exit()
{
    printk("-- YELLOW State EXIT ----------\n");
    printk("YELLOW off\n");
}

void alert_enter()
{
    printk("-- ALERT State ENTER ----------\n");
    printk("YELLOW start blink\n");
}

void alert_exit()
{
    printk("-- ALERT State EXIT ----------\n");
    printk("YELLOW stop blink\n");
}

state_machine_table_row_t state_machine_table[NUMBER_OF_STATES] = {
    /*GREEN STATE*/ {{GREEN, ALERT}, red_enter, red_exit, K_SECONDS(2)},
    /*RED STATE*/ {{YELLOW, ALERT}, green_enter, green_exit, K_SECONDS(3)},
    /*YELLOW STATE*/ {{RED, ALERT}, yellow_enter, yellow_exit, K_SECONDS(1)},
    /*ALERT STATE*/ {{ALERT, RED}, alert_enter, alert_exit, K_MSEC(200)}};


event_t current_event = TIMEOUT_EVENT;

void expire_func(struct k_timer* timer)
{
    printk(" *** Timeout! ---------------------------------------\n");
    current_event = TIMEOUT_EVENT;
    k_sem_give(&light_sema);
}

K_TIMER_DEFINE(light_timer, expire_func, NULL);

void alert_on()
{
    printk("Alert Toggle!!!");
    current_event = ALERT_EVENT;
    k_timer_stop(&light_timer);
    k_sem_give(&light_sema);
}

void main()
{
    printk("Hello main thread!\n");
    current_event      = TIMEOUT_EVENT;
    int count          = 0;
    state_t last_state = RED;
    while (1) {
        k_sem_take(&light_sema, K_FOREVER);
        last_state    = current_state;
        current_state = state_machine_table[current_state].next_state[current_event];
        if (last_state != current_state) {
            state_machine_table[last_state].exit_action();
            state_machine_table[current_state].enter_action();
        }
        k_timer_start(&light_timer, state_machine_table[current_state].timeout, 0);

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
