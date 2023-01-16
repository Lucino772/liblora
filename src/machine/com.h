#ifndef _LIBLORA_MACHINE_COM_H_
#define _LIBLORA_MACHINE_COM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int liblora_com_open(void* dev);
    int liblora_com_transfer(void* dev, uint8_t* buffer, int len);

    // SPI
    int liblora_spi_open(void* dev);
    int liblora_spi_transfer(void* dev, uint8_t* buffer, int len);

    // USB
    // int liblora_usb_open(void* dev);
    // int liblora_usb_transfer(void* dev, uint8_t* buffer, int len);

#ifdef __cplusplus
}
#endif

#endif