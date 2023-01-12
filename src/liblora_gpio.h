#ifndef _LIBLORA_GPIO_H_
#define _LIBLORA_GPIO_H_

// The implementation can be found in the frameworks/ directory.

#include <stdint.h>

#define INPUT       0
#define OUTPUT      1
#define LOW         0
#define HIGH        1

#ifdef __cplusplus
extern "C"
{
#endif

    int liblora_gpio_setup();
    int liblora_gpio_mode(int pin, int mode);
    int liblora_gpio_write(int pin, int val);
    int liblora_gpio_read(int pin, int* val);

#ifdef __cplusplus
}
#endif

#endif