#include "es_button.h"


es_button_t es_button_create(const char* dev_name, u32_t pin,
                             es_button_callback_t callback)
{
    es_button_t button = {.dev = device_get_binding(dev_name), .pin = pin};
    gpio_pin_configure(button.dev, pin,
                       GPIO_DIR_IN | GPIO_PUD_PULL_UP | GPIO_INT | GPIO_INT_DEBOUNCE
                           | GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH);
    gpio_init_callback(&button.gpio_cb, callback, BIT(pin));
    gpio_add_callback(button.dev, &button.gpio_cb);
    gpio_pin_enable_callback(button.dev, pin);
    return button;
}
