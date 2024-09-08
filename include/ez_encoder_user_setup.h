#ifndef __EZ_ENCODER_USER_SETUP_H__
#define __EZ_ENCODER_USER_SETUP_H__

/**
 * Should be changed depending on the number of encoders used.
 */
#define NUMBER_OF_ENCODERS          1

#define ESP32

#ifdef ESP32
#include "driver/gpio.h"
#define encoder_read_gpio(x)        (gpio_get_level((gpio_num_t)x))
#define encoder_disable_intr()      do{}while(0)
#define encoder_enable_intr()       do{}while(0)
#else
#define encoder_read_gpio(x)
#define encoder_disable_intr()      do{}while(0)
#define encoder_enable_intr()       do{}while(0)
#endif

#endif /*__EZ_ENCODER_USER_SETUP_H__*/