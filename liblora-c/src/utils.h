#ifndef _LIBLORA_UTILS_H_
#define _LIBLORA_UTILS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

uint8_t wbits(uint8_t mask, uint8_t val);
uint8_t rbits(uint8_t mask, uint8_t val);

#ifdef __cplusplus
}
#endif

#endif