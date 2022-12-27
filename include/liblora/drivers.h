/*
 * Created on Tue Sep 17 2022
 *
 * Copyright (c) 2022 Lucino772
 */

#ifndef _LIBLORA_PUBLIC_DRIVERS_H_
#define _LIBLORA_PUBLIC_DRIVERS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    #define LIBLORA_DRIVER_WIRINGPI_INIT { 0, 6, 500000 };

    typedef struct
    {
        int spi_dev;
        int spi_ss;
        int spi_clck_speed;
    } liblora_wiringpi_config_t;

#ifdef __cplusplus
}
#endif

#endif