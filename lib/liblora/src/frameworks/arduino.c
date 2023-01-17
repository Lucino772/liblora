#include <stdlib.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.H>

#include "../machine/com.h"
#include "../machine/gpio.h"
#include "../machine/time.h"

// gpio.h
int liblora_gpio_setup()
{
    // Nothing to do
}

int liblora_gpio_mode(int pin, int mode)
{
    pinMode(pin, mode);
    return 0;
}

int liblora_gpio_write(int pin, int val)
{
    digitalWrite(pin, val);
    return 0;
}

int liblora_gpio_read(int pin, int *val)
{
    *val = (uint8_t)digitalRead(pin);
    return 0;
}

int liblora_gpio_interrupt(int pin, int edge_type, void (*callback)(void))
{
    attachInterrupt(digitalPinToInterrupt(pin), callback, edge_type);
#ifdef SPI_HAS_TRANSACTION
    SPI.usingInterrupt(digitalPinToInterrupt(pin));
#endif
    return 0;
}

// com.h
int liblora_spi_open(liblora_com_dev_t *dev)
{
    liblora_gpio_mode(dev->spi_ss, OUTPUT);
    SPI.begin();
    return 0;
}

int liblora_spi_transfer(liblora_com_dev_t *dev, uint8_t *buffer, int len)
{
    // FIXME: Check if transfer returns something
    liblora_gpio_write(dev->spi_ss, LOW);
    SPI.beginTransaction(SPISettings(8e6, MSBFIRST, SPI_MODE0));
    SPI.transfer(buffer, len);
    SPI.endTransaction()
        liblora_gpio_write(dev->spi_ss, HIGH);

    return len;
}

// time.h
void liblora_time_wait(int ms)
{
    delay(ms);
}
