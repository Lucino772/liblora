/*
 * Created on Tue Sep 14 2022
 *
 * Copyright (c) 2022 Lucino772
 */

#ifndef _LIBLORA_PRIVATE_DRIVERS_H_
#define _LIBLORA_PRIVATE_DRIVERS_H_

#include "../include/liblora/drivers.h"

#define INPUT       0
#define OUTPUT      1
#define LOW         0
#define HIGH        1

#define liblora_driver_setup(driver) driver->setup(driver->config)
#define liblora_driver_pin_mode(driver, pin, sel) driver->pin_mode(pin, sel, driver->config)
#define liblora_driver_pin_write(driver, pin, val) driver->pin_write(pin, val, driver->config)
#define liblora_driver_pin_read(driver, pin) driver->pin_read(pin, driver->config)
#define liblora_driver_delay(driver, ms) driver->delay(ms, driver->config)

#define liblora_driver_spi_init(driver) driver->spi_init(driver->config)
#define liblora_driver_spi_read(driver, reg) driver->spi_read(reg, driver->config)
#define liblora_driver_spi_read_burst(driver, reg, buffer, len) driver->spi_read_burst(reg, buffer, len, driver->config)
#define liblora_driver_spi_write(driver, reg, val) driver->spi_write(reg, val, driver->config)
#define liblora_driver_spi_write_burst(driver, reg, buffer, len) driver->spi_write_burst(reg, buffer, len, driver->config)

#endif