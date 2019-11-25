#include "es_led.h"

#include <gpio.h>

#define es_led_update(led) gpio_pin_write(led->dev, led->pin, led->value)
#define es_led_update_ninja(led, v)                     \
    do {                                                \
        led->value = v;                                 \
        gpio_pin_write(led->dev, led->pin, led->value); \
    } while (0)

void es_led_expiry_function(struct k_timer* timer_id)
{
    printk("Triggered!\n");
    es_led_t* led = CONTAINER_OF(timer_id, struct led_s, timer);
    es_led_toggle(led);
}

es_led_t es_led_create(const char* dev_name, u32_t pin, struct k_timer* timer)
{
    es_led_t led = {
        .dev = device_get_binding(dev_name), .pin = pin, .value = 0, .timer = *timer};
    gpio_pin_configure(led.dev, pin, GPIO_DIR_OUT | GPIO_POL_INV);
    es_led_off(&led);
    return led;
}

// static void es_led_update(es_led_t* led)
// {
//     gpio_pin_write(led->dev, led->pin, led->value);
// }

void es_led_on(es_led_t* led)
{
    led->value = 1;
    es_led_update(led);
}

void es_led_off(es_led_t* led)
{
    led->value = 0;
    es_led_update(led);
}

void es_led_toggle(es_led_t* led)
{
    es_led_update_ninja(led, !led->value);
}


// void es_led_blink_start(es_led_t* led, s32_t period_ms, s32_t on_ms)
// {
//     printk("Blink started!\n");
//     k_timer_start(&led->timer, K_MSEC(500), K_MSEC(500));
// }

// void es_led_blink_stop(es_led_t* led)
// {
//     printk("Blink stopped!\n");
//     k_timer_stop(&led->timer);
// }