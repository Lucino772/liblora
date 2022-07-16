#ifndef _LIBLORA_SPI_H_
#define _LIBLORA_SPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

    int spi_driver_init(int dev, int ss, int clck_speed);

    static void _spi_driver_select(int ss);
    static void _spi_driver_unselect(int ss);
    static void _spi_read_write(int dev, int ss, uint8_t *buffer, int len);

    uint8_t spi_read(int dev, int ss, uint8_t reg);
    void spi_read_burst(int dev, int ss, uint8_t reg, uint8_t *buffer, int len);
    void spi_write(int dev, int ss, uint8_t reg, uint8_t val);
    void spi_write_burst(int dev, int ss, uint8_t reg, uint8_t *buffer, int len);

#ifdef __cplusplus
}
#endif

#endif
