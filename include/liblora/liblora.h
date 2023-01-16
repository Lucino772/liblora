#ifndef _LIBLORA_LIBLORA_H_
#define _LIBLORA_LIBLORA_H_

#define LIBLORA_SPI_COM_DEV_INIT { 0, 6, 500000, false }

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        int spi_dev;
        int spi_ss;
        int spi_clck_speed;
        bool usb;
    } liblora_com_dev_t;

#ifdef __cplusplus
}
#endif

#endif