/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <gpio.h>
#include <zephyr.h>

#include "es_button.h"
// #include "es_led.h"
#include "es_version.h"

// #define LED_DEVICE LED0_GPIO_CONTROLLER
// #define LED_PIN LED0_GPIO_PIN
// #define LED_PIN1 LED1_GPIO_PIN
// #define LED_PIN2 LED2_GPIO_PIN
// #define LED_PIN3 LED3_GPIO_PIN

#define BUTTON_DEVICE SW0_GPIO_CONTROLLER
#define BUTTON_PIN SW0_GPIO_PIN
#define BUTTON_PIN1 SW1_GPIO_PIN
#define BUTTON_PIN2 SW2_GPIO_PIN

/* 1000 msec = 1 sec */
#define SLEEP_TIME 1000


// es_led_t led2;
// void es_led2_expiry_function(struct k_timer *timer_id)
// {
//     printk("Triggered!\n");
//     es_led_toggle(&led2);
// }
// K_TIMER_DEFINE(my_timer, es_led2_expiry_function, NULL);
// static struct gpio_callback gpio_cb;

// void button_pressed(struct device *gpiob, struct gpio_callback *cb, u32_t pins)
// {
//     u32_t data  = 0;
//     u8_t button = 1;
//     if (pins & BIT(BUTTON_PIN2)) {
//         gpio_pin_read(gpiob, BUTTON_PIN2, &data);
//         button = 2;
//     } else {
//         gpio_pin_read(gpiob, BUTTON_PIN1, &data);
//         button = 1;
//     }
//     if (data) {
//         printk("Button %d released\n", button);
//     } else {
//         printk("Button %d pressed\n", button);
//     }
// }

void button_callback(struct device *gpiob, struct gpio_callback *cb, u32_t pins)
{
    printk("Button PIN1 pressed!\n");
}


void main(void)
{
    printk("Firmware version: %d.%d-%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
    // es_led_t led  = es_led_create(LED_DEVICE, LED_PIN, NULL);
    // es_led_t led1 = es_led_create(LED_DEVICE, LED_PIN1, NULL);
    // led2          = es_led_create(LED_DEVICE, LED_PIN2, &my_timer);
    // es_led_t led3 = es_led_create(LED_DEVICE, LED_PIN3, NULL);
    // es_led_on(&led1);
    es_button_t button1 = es_button_create(BUTTON_DEVICE, BUTTON_PIN1, button_callback);
    while (1) {
        printk("Hello World!\n");
        k_sleep(K_SECONDS(1));
    }
}
