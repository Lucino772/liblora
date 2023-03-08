#ifdef LIBLORA_FRAMEWORK_WIRINGPI

#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "liblora/core/board.h"

// gpio.h
int liblora_board_gpio_setup(void* userdata)
{
    return wiringPiSetup();
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

int liblora_board_gpio_read(int pin, int* val, void* userdata)
{
    *val = (uint8_t)digitalRead(pin);
    return 0;
}

int liblora_board_gpio_attach_interrupt(int pin, int edge_type, void (*callback)(void), void* userdata)
{
    wiringPiISR(pin, edge_type, callback);
    return 0;
}

// com.h
int liblora_board_spi_open(liblora_dev_t* dev, void* userdata)
{
    liblora_board_gpio_mode(dev->spi_ss, OUTPUT, userdata);
    return wiringPiSPISetup(dev->spi_dev, dev->spi_clck_speed);
}

int liblora_board_spi_transfer(liblora_dev_t* dev, uint8_t* buffer, int len, void* userdata)
{
    liblora_board_gpio_write(dev->spi_ss, LIBLORA_BOARD_GPIO_STATE_LOW, userdata);
    int nread = wiringPiSPIDataRW(dev->spi_dev, buffer, len);
    liblora_board_gpio_write(dev->spi_ss, LIBLORA_BOARD_GPIO_STATE_HIGH, userdata);

    if (nread > 0)
        return nread;

    return -1;
}

// time.h
void liblora_board_wait(int ms, void* userdata)
{
    delay(ms);
}

#endif