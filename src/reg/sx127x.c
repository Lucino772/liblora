#include <string.h>

#include "sx127x.h"
#include "../machine/com.h"


int liblora_reg_sx127x_r(void* dev, uint8_t addr, uint8_t* val)
{
    uint8_t buffer[2];
    buffer[0] = addr & 0x7F;
    buffer[1] = 0x00;

    liblora_com_transfer(dev, buffer, 2);
    *val = buffer[1];

    return 0;
}

int liblora_reg_sx127x_rb(void* dev, uint8_t addr, uint8_t* buf, int len)
{
    uint8_t buffer[len+1];

    memset(buffer, 0, sizeof(buffer));
    buffer[0] = addr & 0x7F;

    liblora_com_transfer(dev, buffer, len+1);
    memcpy(buf, &buffer[1], len);
    return 0;
}

int liblora_reg_sx127x_w(void* dev, uint8_t addr, uint8_t val)
{
    uint8_t buffer[2];
    buffer[0] = addr | 0x80;
    buffer[1] = val;

    liblora_com_transfer(dev, buffer, 2);
    return 0;
}

int liblora_reg_sx127x_wb(void* dev, uint8_t addr, uint8_t* buf, int len)
{
    uint8_t buffer[len+1];

    memset(buffer, 0, sizeof(buffer));
    buffer[0] = addr & 0x80;
    for (int i = 0; i < len; i++)
        buffer[i + 1] = buf[i];

    liblora_com_transfer(dev, buffer, len+1);
    return 0;
}
