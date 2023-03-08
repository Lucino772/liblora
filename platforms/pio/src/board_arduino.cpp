#ifdef LIBLORA_FRAMEWORK_ARDUINO

#include <stdlib.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>

#include "liblora/core/board.h"

// gpio.h
int liblora_board_gpio_setup(void* userdata)
{
    // Nothing to do
}

int liblora_board_gpio_mode(int pin, int mode, void* userdata)
{
    pinMode(pin, mode);
    return 0;
}

int liblora_board_gpio_write(int pin, int val, void* userdata)
{
    digitalWrite(pin, val);
    return 0;
}

int liblora_board_gpio_read(int pin, int *val, void* userdata)
{
    *val = (uint8_t)digitalRead(pin);
    return 0;
}

int liblora_board_gpio_attach_interrupt(int pin, int edge_type, void (*callback)(void), void* userdata)
{
    attachInterrupt(digitalPinToInterrupt(pin), callback, edge_type);
#ifdef SPI_HAS_TRANSACTION
    SPI.usingInterrupt(digitalPinToInterrupt(pin));
#endif
    return 0;
}

// com.h
int liblora_board_spi_open(liblora_dev_t *dev, void* userdata)
{
    liblora_board_gpio_mode(dev->spi_ss, LIBLORA_BOARD_GPIO_MODE_OUTPUT, userdata);
    SPI.begin();
    return 0;
}

int liblora_board_spi_transfer(liblora_dev_t *dev, uint8_t *buffer, int len, void* userdata)
{
    // FIXME: Check if transfer returns something
    liblora_board_gpio_write(dev->spi_ss, LIBLORA_BOARD_GPIO_STATE_LOW, userdata);
    SPI.beginTransaction(SPISettings(8e6, MSBFIRST, SPI_MODE0));
    SPI.transfer(buffer, len);
    SPI.endTransaction();
    liblora_board_gpio_write(dev->spi_ss, LIBLORA_BOARD_GPIO_STATE_HIGH, userdata);

    return len;
}

// time.h
void liblora_board_wait(int ms, void* userdata)
{
    delay(ms);
}

#endif