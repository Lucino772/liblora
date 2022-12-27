/*
 * Created on Mon Dec 27 2022
 *
 * Copyright (c) 2022 Lucino772
 */

#ifndef _LIBLORA_PRIVATE_DRIVER_H_
#define _LIBLORA_PRIVATE_DRIVER_H_

#include <stdint.h>
#include "../include/liblora/drivers.h"

// Pins types
#define INPUT       0
#define OUTPUT      1
#define LOW         0
#define HIGH        1

// Interrupt levels
#define	INT_EDGE_SETUP      0
#define	INT_EDGE_FALLING    1
#define	INT_EDGE_RISING     2
#define	INT_EDGE_BOTH       3

#ifdef __cplusplus
extern "C"
{
#endif
    // setup
    int liblora_driver_setup(void *config);

    // pins
    void liblora_driver_pin_mode(int pin, int sel, void *config);
    void liblora_driver_pin_write(int pin, int val, void *config);
    uint8_t liblora_driver_pin_read(int pin, void *config);

    // timing
    void liblora_driver_delay(int ms, void *config);

    // SPI
    int liblora_driver_spi_init(void *config);
    uint8_t liblora_driver_spi_read(uint8_t reg, void *config);
    uint8_t liblora_driver_spi_read_burst(uint8_t reg, uint8_t *buf, int len, void *config);
    void liblora_driver_spi_write(uint8_t reg, uint8_t val, void *config);
    void liblora_driver_spi_write_burst(uint8_t reg, uint8_t *buf, int len, void *config);

    // Interrupts
    void liblora_driver_attach_interrupt(int pin, int edge_type, void (*callback)(int, void*), void *config);
    void liblora_driver_attach_interrupt_ex(int pin, int edge_type, void (*callback)(int, void*), void* userdata, void *config);

#ifdef __cplusplus
}
#endif

#endif