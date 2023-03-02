#ifndef _LIBLORA_CORE_LIB_BOARD_H_
#define _LIBLORA_CORE_LIB_BOARD_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct liblora_com_dev_t {
        int spi_dev;
        int spi_ss;
        int spi_clck_speed;
    } liblora_com_dev_t;

    typedef struct liblora_board_t
    {
        // SPI
        int (*spi_open)(liblora_com_dev_t* dev, void* userdata);
        int (*spi_transfer)(liblora_com_dev_t* dev, uint8_t* buffer, int len, void* userdata);

        // SPIO
        int (*gpio_setup)(void);
        int (*gpio_mode)(int pin, int mode, void* userdata);
        int (*gpio_write)(int pin, int val, void* userdata);
        int (*gpio_read)(int pin, int *val, void* userdata);
        int (*gpio_attach_interrupt)(int pin, int type, void (*callback)(void), void* userdata);

        // TIME
        void (*wait)(int ms);
    } liblora_board_t;

#ifdef __cplusplus
}
#endif

#endif