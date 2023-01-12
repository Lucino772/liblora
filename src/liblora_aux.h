#ifndef _LIBLORA_AUX_H_
#define _LIBLORA_AUX_H_

// The implementation can be found in the frameworks/ directory.

#include <stdint.h>

#define	INT_EDGE_SETUP      0
#define	INT_EDGE_FALLING    1
#define	INT_EDGE_RISING     2
#define	INT_EDGE_BOTH       3

#ifdef __cplusplus
extern "C"
{
#endif

    void liblora_aux_wait(int ms);
    int liblora_aux_interrupt(int pin, int edge_type, void (*callback)(int, void*), void* userdata);
    int liblora_aux_interrupt_ex(int pin, int edge_type, void (*callback)(int, void*));

#ifdef __cplusplus
}
#endif

#endif