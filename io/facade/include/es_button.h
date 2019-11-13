
#ifndef _ES_BUTTON_H_
#define _ES_BUTTON_H_

#include <device.h>
#include <zephyr/types.h>

typedef void (*es_button_callback_t)(struct device*, struct gpio_callback*, u32_t);
typedef struct button_s es_button_t;

struct button_s {
    struct device* dev;
    struct gpio_callback gpio_cb;
    u32_t pin;
};


void es_button_init(es_button_t* button, struct device* dev, u32_t pin,
                    es_button_callback_t callback);


#endif /*_ES_BUTTON_H_*/