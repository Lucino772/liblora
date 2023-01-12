#include "liblora_com.h"
#include "liblora_spi.h"

int liblora_com_open(liblora_com_t* com)
{
    switch (com->type)
    {
    case LIBLORA_COM_TARGET_SPI:
        return liblora_spi_open(com->target);
    default:
        break;
    }

    return -1;
}

int liblora_com_close(liblora_com_t* com)
{
    switch (com->type)
    {
    case LIBLORA_COM_TARGET_SPI:
        return liblora_spi_close(com->target);
    default:
        break;
    }

    return -1;
}


int liblora_com_r(liblora_com_t* com, uint8_t addr, uint8_t* val)
{
    switch (com->type)
    {
    case LIBLORA_COM_TARGET_SPI:
        return liblora_spi_r(com->target, addr, val);
    default:
        break;
    }

    return -1;
}

int liblora_com_rb(liblora_com_t* com, uint8_t addr, uint8_t *buffer, int len)
{
    switch (com->type)
    {
    case LIBLORA_COM_TARGET_SPI:
        return liblora_spi_rb(com->target, addr, buffer, len);
    default:
        break;
    }

    return -1;
}


int liblora_com_w(liblora_com_t* com, uint8_t addr, uint8_t val)
{
    switch (com->type)
    {
    case LIBLORA_COM_TARGET_SPI:
        return liblora_spi_w(com->target, addr, val);
    default:
        break;
    }

    return -1;
}

int liblora_com_wb(liblora_com_t* com, uint8_t addr, uint8_t *buffer, int len)
{
    switch (com->type)
    {
    case LIBLORA_COM_TARGET_SPI:
        return liblora_spi_wb(com->target, addr, buffer, len);
    default:
        break;
    }

    return -1;
}

