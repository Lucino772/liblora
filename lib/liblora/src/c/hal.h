#ifndef _LIBLORA_MACHINE_H_
#define _LIBLORA_MACHINE_H_

#include <stdint.h>
#include <stdbool.h>

#include "../../include/liblora/liblora.h"

#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

#define INT_EDGE_SETUP 0
#define INT_EDGE_FALLING 1
#define INT_EDGE_RISING 2
#define INT_EDGE_BOTH 3

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        int spi_dev;
        int spi_ss;
        int spi_clck_speed;
        bool usb;
    } liblora_com_dev_t;

    // SPI
    int liblora_spi_open(liblora_com_dev_t *dev);
    int liblora_spi_transfer(liblora_com_dev_t *dev, uint8_t *buffer, int len);

    // GPIO
    int liblora_gpio_setup();
    int liblora_gpio_mode(int pin, int mode);
    int liblora_gpio_write(int pin, int val);
    int liblora_gpio_read(int pin, int *val);
    int liblora_gpio_interrupt(int pin, int edge_type, void (*callback)(void));

    // TIME
    void liblora_time_wait(int ms);

#ifdef __cplusplus
}
#endif

#endif