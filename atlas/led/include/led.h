#ifndef __LED_H_
#define __LED_H_

#include <gpio.h>

#define N_GPIO 3

void led_show(gpio &g, int led_idx);

#endif

