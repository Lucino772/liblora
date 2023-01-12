#ifndef _LIBLORA_SPI_H_
#define _LIBLORA_SPI_H_

// The implementation can be found in the frameworks/ directory.

#include <stdint.h>
#include "../include/liblora/liblora.h"

#ifdef __cplusplus
extern "C"
{
#endif

    int liblora_spi_open(void* target);
    int liblora_spi_close(void* target);

    int liblora_spi_r(void* target, uint8_t addr, uint8_t* val);
    int liblora_spi_rb(void* target, uint8_t addr, uint8_t *buffer, int len);
    
    int liblora_spi_w(void* target, uint8_t addr, uint8_t val);
    int liblora_spi_wb(void* target, uint8_t addr, uint8_t *buffer, int len);

#ifdef __cplusplus
}
#endif

#endif