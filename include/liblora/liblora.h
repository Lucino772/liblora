#ifndef _LIBLORA_LIBLORA_H_
#define _LIBLORA_LIBLORA_H_

#define LIBLORA_SPI_WIRINGPI_INIT { 0, 6, 500000 };

#define LIBLORA_COM_TARGET_SPI 0
// #define LIBLORA_COM_TARGET_USB 1

#define LIBLORA_COM_SPI(target) {LIBLORA_COM_TARGET_SPI, target}
// #define LIBLORA_COM_USB(target) {LIBLORA_COM_TARGET_USB, target}


#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        int type;
        void* target;
    } liblora_com_t;

    typedef struct
    {
        int spi_dev;
        int spi_ss;
        int spi_clck_speed;
    } liblora_spi_target_t;

#ifdef __cplusplus
}
#endif

#endif