/*
 * Created on Tue Sep 14 2022
 *
 * Copyright (c) 2022 Lucino772
 */

#include "_drivers.h"

#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>


typedef struct {
    int spi_dev;
    int spi_ss;
    int spi_clck_speed;
} _wiringpi_ops_t;

// GPIO
static int _wiringpi_setup(void* config)
{
    return wiringPiSetup();
}

static void _wiringpi_pin_mode(int pin, int sel, void* config)
{
    pinMode(pin, sel);
}

static uint8_t _wiringpi_pin_read(int pin, void* config)
{
    return (uint8_t)digitalRead(pin);
}

static void _wiringpi_pin_write(int pin, int val, void* config)
{
    digitalWrite(pin, val);
}


// Timing
static void _wiringpi_delay(int ms, void* config)
{
    delay(ms);
}


// SPI
static int _wiringpi_spi_rw(uint8_t *buffer, int len, void* config)
{
    _wiringpi_ops_t* ops = (_wiringpi_ops_t*)config;
    _wiringpi_pin_write(ops->spi_ss, LOW, config);
    int nread = wiringPiSPIDataRW(ops->spi_dev, buffer, len);
    _wiringpi_pin_write(ops->spi_ss, HIGH, config);

    return nread;
}

static int _wiringpi_spi_init(void* config)
{
    _wiringpi_ops_t* ops = (_wiringpi_ops_t*)config;
    _wiringpi_pin_mode(ops->spi_ss, OUTPUT, config);
    return wiringPiSPISetup(ops->spi_dev, ops->spi_clck_speed);
}

static uint8_t _wiringpi_spi_read(uint8_t reg, void* config) {
    uint8_t buffer[2];
    buffer[0] = reg & 0x7F;
    buffer[1] = 0x00;

    _wiringpi_spi_rw(buffer, 2, config);
    return buffer[1];
}

static uint8_t _wiringpi_spi_read_burst(uint8_t reg, uint8_t *buf, int len, void* config)
{
    uint8_t *buffer = (uint8_t *)calloc(len + 1, sizeof(uint8_t));
    buffer[0] = reg & 0x7F;
    _wiringpi_spi_rw(buffer, 2, config);

    memcpy(buf, buffer++, len);
    // free(buffer);
}

static void _wiringpi_spi_write(uint8_t reg, uint8_t val, void* config)
{
    uint8_t buffer[2];
    buffer[0] = reg | 0x80;
    buffer[1] = val;

    _wiringpi_spi_rw(buffer, 2, config);
}

static void _wiringpi_spi_write_burst(uint8_t reg, uint8_t *buf, int len, void* config)
{
    uint8_t *buffer = (uint8_t *)calloc(len + 1, sizeof(uint8_t));
    buffer[0] = reg & 0x80;
    for (int i = 0; i < len; i++)
        buffer[i + 1] = buf[i];

    _wiringpi_spi_rw(buffer, len + 1, config);

    // free(buffer);
}


liblora_driver_t* liblora_driver_wiringpi(int spi_dev, int spi_ss)
{
    _wiringpi_ops_t* ops = (_wiringpi_ops_t*)malloc(sizeof(_wiringpi_ops_t));
    ops->spi_dev = spi_dev;
    ops->spi_ss = spi_ss;
    ops->spi_clck_speed = 500000;

    liblora_driver_t* driver = (liblora_driver_t*)malloc(sizeof(liblora_driver_t));

    driver->config = ops;
    driver->setup = _wiringpi_setup;
    driver->pin_mode = _wiringpi_pin_mode;
    driver->pin_write = _wiringpi_pin_write;
    driver->pin_read = _wiringpi_pin_read;
    driver->delay = _wiringpi_delay;
    driver->spi_init = _wiringpi_spi_init;
    driver->spi_read = _wiringpi_spi_read;
    driver->spi_read_burst = _wiringpi_spi_read_burst;
    driver->spi_write = _wiringpi_spi_write;
    driver->spi_write_burst = _wiringpi_spi_write_burst;

    return driver;
}
