#include "./spi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

int spi_driver_init(int dev, int ss, int clck_speed)
{
    pinMode(ss, OUTPUT);
    if (wiringPiSPISetup(dev, clck_speed) == -1)
    {
        perror("Faile to initialize SPI");
        return -1;
    }

    return 0;
}

static void _spi_driver_select(int ss)
{
    digitalWrite(ss, LOW);
}

static void _spi_driver_unselect(int ss)
{
    digitalWrite(ss, HIGH);
}

static void _spi_read_write(int dev, int ss, uint8_t *buffer, int len)
{
    _spi_driver_select(ss);
    wiringPiSPIDataRW(dev, buffer, len);
    _spi_driver_unselect(ss);
}

uint8_t spi_read(int dev, int ss, uint8_t reg)
{
    uint8_t buffer[2];
    buffer[0] = reg & 0x7F;
    buffer[1] = 0x00;

    _spi_read_write(dev, ss, buffer, 2);
    return buffer[1];
}

void spi_read_burst(int dev, int ss, uint8_t reg, uint8_t *buf, int len)
{
    uint8_t *buffer = (uint8_t *)calloc(len + 1, sizeof(uint8_t));
    buffer[0] = reg & 0x7F;
    _spi_read_write(dev, ss, buffer, 2);

    memcpy(buf, buffer++, len);
    free(buffer);
}

void spi_write(int dev, int ss, uint8_t reg, uint8_t val)
{
    uint8_t buffer[2];
    buffer[0] = reg | 0x80;
    buffer[1] = val;

    _spi_read_write(dev, ss, buffer, 2);
}

void spi_write_burst(int dev, int ss, uint8_t reg, uint8_t *buf, int len)
{
    uint8_t *buffer = (uint8_t *)calloc(len + 1, sizeof(uint8_t));
    buffer[0] = reg & 0x80;
    for (int i = 0; i < len; i++)
        buffer[i + 1] = buf[i];

    _spi_read_write(dev, ss, buffer, 2);

    free(buffer);
}
