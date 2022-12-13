/*
 * Created on Tue Sep 17 2022
 *
 * Copyright (c) 2022 Lucino772
 */

#ifndef _LIBLORA_PUBLIC_DRIVERS_H_
#define _LIBLORA_PUBLIC_DRIVERS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

    typedef struct
    {
        void *config;

        // GPIO
        int (*setup)(void *);
        void (*pin_mode)(int, int, void *);
        void (*pin_write)(int, int, void *);
        uint8_t (*pin_read)(int, void *);
        void (*delay)(int, void *);

        // SPI
        int (*spi_init)(void *);
        uint8_t (*spi_read)(uint8_t, void *);
        uint8_t (*spi_read_burst)(uint8_t, uint8_t *, int, void *);
        void (*spi_write)(uint8_t, uint8_t, void *);
        void (*spi_write_burst)(uint8_t, uint8_t *, int, void *);

        // Interrupts
        void (*attach_interrupt)(int, int, void (*)(int), void*);
    } liblora_driver_t;

    liblora_driver_t *liblora_driver_wiringpi(int spi_dev, int spi_ss);
    // liblora_driver_t* liblora_driver_bcm2835(int spi_ss);

    void liblora_driver_delete(liblora_driver_t *driver); // TODO
#ifdef __cplusplus
}
#endif

#endif