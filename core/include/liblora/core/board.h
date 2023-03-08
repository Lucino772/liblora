#ifndef _LIBLORA_CORE_LIB_BOARD_H_
#define _LIBLORA_CORE_LIB_BOARD_H_

#include <stdint.h>

#define LIBLORA_BOARD_GPIO_MODE_INPUT       0
#define LIBLORA_BOARD_GPIO_MODE_OUTPUT      1

#define LIBLORA_BOARD_GPIO_STATE_LOW        0
#define LIBLORA_BOARD_GPIO_STATE_HIGH       1

#define LIBLORA_BOARD_GPIO_EDGE_SETUP       0
#define LIBLORA_BOARD_GPIO_EDGE_FALLING     1
#define LIBLORA_BOARD_GPIO_EDGE_RISING      2
#define LIBLORA_BOARD_GPIO_EDGE_BOTH        3

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct liblora_dev_t {
        int spi_dev;
        int spi_ss;
        int spi_clck_speed;
    } liblora_dev_t;

    // SPI
    int liblora_board_spi_open(liblora_dev_t* dev, void* userdata);
    int liblora_board_spi_transfer(liblora_dev_t* dev, uint8_t* buffer, int len, void* userdata);

    // GPIO
    int liblora_board_gpio_setup(void* userdata);
    int liblora_board_gpio_mode(int pin, int mode, void* userdata);
    int liblora_board_gpio_write(int pin, int val, void* userdata);
    int liblora_board_gpio_read(int pin, int *val, void* userdata);
    int liblora_board_gpio_attach_interrupt(int pin, int type, void (*callback)(void), void* userdata);

    // TIME
    void liblora_board_wait(int ms, void* userdata);

#ifdef __cplusplus
}
#endif

#endif