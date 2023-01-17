#ifndef _LIBLORA_MACHINE_GPIO_H_
#define _LIBLORA_MACHINE_GPIO_H_

#include <stdint.h>

#define INPUT       0
#define OUTPUT      1
#define LOW         0
#define HIGH        1

#define	INT_EDGE_SETUP      0
#define	INT_EDGE_FALLING    1
#define	INT_EDGE_RISING     2
#define	INT_EDGE_BOTH       3

#ifdef __cplusplus
extern "C"
{
#endif

    int liblora_gpio_setup();
    int liblora_gpio_mode(int pin, int mode);
    int liblora_gpio_write(int pin, int val);
    int liblora_gpio_read(int pin, int* val);

    int liblora_gpio_interrupt(int pin, int edge_type, void (*callback)(void));

#ifdef __cplusplus
}
#endif

#endif