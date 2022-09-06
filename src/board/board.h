/*
 * Created on Tue Sep 6 2022
 *
 * Copyright (c) 2022 Lucino772
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

    int board_setup();
    int board_pin_mode(int pin, int mode);
    void board_digital_write(int pin, int val);
    void board_delay(int ms);

    int board_spi_init(int dev, int ss, int clck_speed);
    uint8_t board_spi_read(int dev, int ss, uint8_t reg);
    void board_spi_read_burst(int dev, int ss, uint8_t reg, uint8_t *buffer, int len);
    void board_spi_write(int dev, int ss, uint8_t reg, uint8_t val);
    void board_spi_write_burst(int dev, int ss, uint8_t reg, uint8_t *buffer, int len);

#ifdef __cplusplus
}
#endif

#endif