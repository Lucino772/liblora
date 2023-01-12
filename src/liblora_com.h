#ifndef _LIBLORA_COM_H_
#define _LIBLORA_COM_H_

#include <stdint.h>

#include "../include/liblora/liblora.h"

#ifdef __cplusplus
extern "C"
{
#endif

    int liblora_com_open(liblora_com_t *com);
    int liblora_com_close(liblora_com_t *com);

    int liblora_com_r(liblora_com_t *com, uint8_t addr, uint8_t *val);
    int liblora_com_rb(liblora_com_t *com, uint8_t addr, uint8_t *buffer, int len);

    int liblora_com_w(liblora_com_t *com, uint8_t addr, uint8_t val);
    int liblora_com_wb(liblora_com_t *com, uint8_t addr, uint8_t *buffer, int len);

#ifdef __cplusplus
}
#endif

#endif