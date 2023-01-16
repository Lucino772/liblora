#include "com.h"

int liblora_com_open(liblora_com_dev_t* dev)
{
    // if (dev->usb) {
    //     return liblora_usb_open(dev);
    // }

    return liblora_spi_open(dev);
}

int liblora_com_transfer(liblora_com_dev_t* dev, uint8_t* buffer, int len)
{
    // if (dev->usb) {
    //     return liblora_usb_transfer(dev, buffer, len);
    // }

    return liblora_spi_transfer(dev, buffer, len);
}
