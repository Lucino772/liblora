#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "../machine/com.h"
#include "../machine/gpio.h"
#include "../machine/time.h"

// gpio.h
int liblora_gpio_setup()
{
    return wiringPiSetup();
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

int liblora_gpio_read(int pin, int* val)
{
    *val = (uint8_t)digitalRead(pin);
    return 0;
}

int liblora_gpio_interrupt(int pin, int edge_type, void (*callback)(void))
{
    wiringPiISR(pin, edge_type, callback);
    return 0;
}

// com.h
int liblora_spi_open(liblora_com_dev_t* dev)
{
    liblora_gpio_mode(dev->spi_ss, OUTPUT);
    return wiringPiSPISetup(dev->spi_dev, dev->spi_clck_speed);
}

int liblora_spi_transfer(liblora_com_dev_t* dev, uint8_t* buffer, int len)
{
    liblora_gpio_write(dev->spi_ss, LOW);
    int nread = wiringPiSPIDataRW(dev->spi_dev, buffer, len);
    liblora_gpio_write(dev->spi_ss, HIGH);

    if (nread > 0)
        return nread;

    return -1;
}

// time.h
void liblora_time_wait(int ms)
{
    delay(ms);
}
