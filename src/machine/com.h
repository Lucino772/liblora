#ifndef _LIBLORA_MACHINE_COM_H_
#define _LIBLORA_MACHINE_COM_H_

#include <stdint.h>
#include <stdbool.h>

#include "../../include/liblora/liblora.h"

#ifdef __cplusplus
extern "C"
{
#endif

    int liblora_com_open(liblora_com_dev_t* dev);
    int liblora_com_transfer(liblora_com_dev_t* dev, uint8_t* buffer, int len);

    // SPI
    int liblora_spi_open(liblora_com_dev_t* dev);
    int liblora_spi_transfer(liblora_com_dev_t* dev, uint8_t* buffer, int len);

    // USB
    // int liblora_usb_open(liblora_com_dev_t* dev);
    // int liblora_usb_transfer(liblora_com_dev_t* dev, uint8_t* buffer, int len);

#ifdef __cplusplus
}
#endif

#endif