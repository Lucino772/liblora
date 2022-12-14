#include "utils.h"

static int _get_shift(uint8_t mask)
{
    int shift = 0;
    while (((mask >> shift) & 0x1) == 0)
        shift++;

    return shift;
}

uint8_t wbits(uint8_t mask, uint8_t val)
{
    if (mask == 0)
        return 0;

    return (val << _get_shift(mask)) & mask;
}

uint8_t rbits(uint8_t mask, uint8_t val)
{
    if (mask == 0)
        return 0;

    return (val & mask) >> _get_shift(mask);
}
