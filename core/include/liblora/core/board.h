/**
 * @file board.h
 * @author Lucino772
 * @date 8 Mars 2023
 */

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

    /**
     * @struct liblora_dev_t
     * @brief This struct represents a SPI device
    */
    typedef struct liblora_dev_s 
    {
        int spi_dev;
        int spi_ss;
        int spi_clck_speed;
    } liblora_dev_t;

    /**
     * @param dev
     * @param userdata
    */
    int liblora_board_spi_open(liblora_dev_t* dev, void* userdata);

    /**
     * @brief Transfer a buffer of bytes with SPI
     * @param dev
     * @param buffer
     * @param len
     * @param userdata
    */
    int liblora_board_spi_transfer(liblora_dev_t* dev, uint8_t* buffer, int len, void* userdata);

    /**
     * @brief GPIO setup
     * @param userdata
    */
    int liblora_board_gpio_setup(void* userdata);

    /**
     * @brief Setup GPIO mode
     * @param pin
     * @param mode LIBLORA_BOARD_GPIO_MODE_INPUT or LIBLORA_BOARD_GPIO_MODE_OUTPUT
     * @param userdata
    */
    int liblora_board_gpio_mode(int pin, int mode, void* userdata);

    /**
     * @brief Write to GPIO
     * @param pin
     * @param val LIBLORA_BOARD_GPIO_STATE_LOW or LIBLORA_BOARD_GPIO_STATE_HIGH
     * @param userdata
    */
    int liblora_board_gpio_write(int pin, int val, void* userdata);

    /**
     * @brief Read from GPIO
     * @param pin
     * @param val
     * @param userdata
    */
    int liblora_board_gpio_read(int pin, int *val, void* userdata);

    /**
     * @brief Attach interrupt to GPIO
     * @param pin
     * @param type LIBLORA_BOARD_GPIO_EDGE_FALLING, LIBLORA_BOARD_GPIO_EDGE_RISING or LIBLORA_BOARD_GPIO_EDGE_BOTH
     * @param callback
     * @param userdata
    */
    int liblora_board_gpio_attach_interrupt(int pin, int type, void (*callback)(void), void* userdata);

    /**
     * @brief Pause program for a certain amount of miliseconds
     * @param ms
     * @param userdata
    */
    void liblora_board_wait(int ms, void* userdata);

#ifdef __cplusplus
}
#endif

#endif