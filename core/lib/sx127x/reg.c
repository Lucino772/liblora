#include <string.h>

#include "./reg.h"
#include "../board.h"

static int reg_r(void *dev, uint8_t addr, uint8_t *val)
{
    uint8_t buffer[2];
    buffer[0] = addr & 0x7F;
    buffer[1] = 0x00;

    liblora_spi_transfer(dev, buffer, 2);
    *val = buffer[1];
    return 0;
}

static int reg_w(void *dev, uint8_t addr, uint8_t val)
{
    uint8_t buffer[2];
    buffer[0] = addr | 0x80;
    buffer[1] = val;

    liblora_spi_transfer(dev, buffer, 2);
    return 0;
}

static int reg_rmw(void *dev, uint8_t addr, uint8_t offset, uint8_t len, uint8_t val)
{
    uint8_t curr, mask, _new;

    if (offset + val > 8)
        return -1;

    reg_r(dev, addr, &curr);
    mask = ((1 << len) - 1) << offset;
    _new = (curr & ~mask) | ((val << offset) & mask);
    reg_w(dev, addr, _new);

    return 0;
}

static int reg_rb(void *dev, uint8_t addr, uint8_t *buf, int len)
{
    uint8_t buffer[len + 1];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = addr & 0x7F;

    liblora_spi_transfer(dev, buffer, len + 1);
    memcpy(buf, &buffer[1], len);
    return 0;
}

static int reg_wb(void *dev, uint8_t addr, uint8_t *buf, int len)
{
    uint8_t buffer[len + 1];
    memset(buffer, 0, sizeof(buffer));
    memcpy(&buffer[1], buf, len);
    buffer[0] = addr & 0x80;

    liblora_spi_transfer(dev, buffer, len + 1);
    return 0;
}

static int check_valid(liblora_sx127x_reg_t reg, uint8_t mode, bool chunk)
{
    if (reg.addr > 0x70 || reg.addr < 0x00)
        return -1;
    if (reg.offset < 0 || reg.offset > 7)
        return -1;
    if (reg.len < 1 || reg.len > 8)
        return -1;
    if ((reg.mode & mode) == 0)
        return -1;
    if ((reg.offset + reg.len) > 8)
        return -1;
    if (chunk && (reg.offset != 0 || reg.len != 8))
        return -1;

    return 0;
}

static int check_access(void *dev, liblora_sx127x_reg_t reg)
{
    uint8_t opmode, expected;

    if (reg.access != SX127x_REG_SHARED)
    {
        if (reg_r(dev, SX127x_REG_ADDR(LIBLORA_SX127x_REG_OPMODE_LR_MODE), &opmode) != 0)
        {
            return -1;
        }

        // !!! Special Case: LoRa Mode with access to FSK registers
        expected = (opmode & 0x80) > 0 ? SX127x_REG_LORA : SX127x_REG_FSK;
        if ((opmode & 0xC0) > 0 && reg.addr >= 0x0D && reg.addr <= 0x3F)
        {
            expected = SX127x_REG_FSK;
        }

        return (reg.access == expected) > 0 ? 0 : -1;
    }

    return 0;
}

int liblora_sx127x_reg_read(void *dev, uint32_t reg, uint8_t *val)
{
    uint8_t temp, ret;

    liblora_sx127x_reg_t _reg = SX127x_REG_PARSE(reg);
    if (check_valid(_reg, SX127x_REG_MODE_R, false) == -1 || check_access(dev, _reg) == -1)
        return -1;

    ret = reg_r(dev, _reg.addr, &temp);
    *val = (temp >> _reg.offset) & ((1 << _reg.len) - 1);
    return ret;
}

int liblora_sx127x_reg_readb(void *dev, uint32_t reg, uint8_t *buf, int len)
{
    liblora_sx127x_reg_t _reg = SX127x_REG_PARSE(reg);
    if (check_valid(_reg, SX127x_REG_MODE_R, true) == -1 || check_access(dev, _reg) == -1)
        return -1;

    return reg_rb(dev, _reg.addr, buf, len);
}

int liblora_sx127x_reg_write(void *dev, uint32_t reg, uint8_t val)
{
    liblora_sx127x_reg_t _reg = SX127x_REG_PARSE(reg);
    if (check_valid(_reg, SX127x_REG_MODE_W, false) == -1 || check_access(dev, _reg) == -1)
        return -1;

    if (_reg.offset == 0 && _reg.len == 8)
    {
        return reg_w(dev, _reg.addr, val);
    }
    else
    {
        return reg_rmw(dev, _reg.addr, _reg.offset, _reg.len, val);
    }

    return 0;
}

int liblora_sx127x_reg_writeb(void *dev, uint32_t reg, uint8_t *buf, int len)
{
    liblora_sx127x_reg_t _reg = SX127x_REG_PARSE(reg);
    if (check_valid(_reg, SX127x_REG_MODE_W, true) == -1 || check_access(dev, _reg) == -1)
        return -1;

    return reg_wb(dev, _reg.addr, buf, len);
}
