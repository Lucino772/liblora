#ifndef _LIBLORA_REG_SX127x_H_
#define _LIBLORA_REG_SX127x_H_

#include <stdint.h>

/**============================================
 *               REGISTERS
 *=============================================**/
#define LIBLORA_RF95_REG_FIFO                 0x00

#define LIBLORA_RF95_REG_OPMODE               0x01
#define LIBLORA_RF95_REG_FR_MSB               0x06
#define LIBLORA_RF95_REG_FR_MID               0x07
#define LIBLORA_RF95_REG_FR_LSB               0x08

#define LIBLORA_RF95_REG_PA_CONFIG            0x09
#define LIBLORA_RF95_REG_PA_RAMP              0x0A
#define LIBLORA_RF95_REG_OCP                  0x0B
#define LIBLORA_RF95_REG_LNA                  0x0C

#define LIBLORA_RF95_REG_FIFO_ADDR_PTR        0x0D
#define LIBLORA_RF95_REG_FIFO_TX_BASE_ADDR    0x0E
#define LIBLORA_RF95_REG_FIFO_RX_BASE_ADDR    0x0F
#define LIBLORA_RF95_REG_FIFO_RX_CURR_ADDR    0x10

#define LIBLORA_RF95_REG_IRQ_FLAGS_MASK       0x11
#define LIBLORA_RF95_REG_IRQ_FLAGS            0x12

#define LIBLORA_RF95_REG_RX_NB_BYTES          0x13
#define LIBLORA_RF95_REG_RX_HEAD_CNT_VAL_MSB  0x14
#define LIBLORA_RF95_REG_RX_HEAD_CNT_VAL_LSB  0x15
#define LIBLORA_RF95_REG_RX_PKT_CNT_VAL_MSB   0x16
#define LIBLORA_RF95_REG_RX_PKT_CNT_VAL_LSB   0x17

#define LIBLORA_RF95_REG_MODEM_STATUS         0x18
#define LIBLORA_RF95_REG_PKT_SNR_VAL          0x19
#define LIBLORA_RF95_REG_PKT_RSSI_VAL         0x1A
#define LIBLORA_RF95_REG_RSSI_VAL             0x1B

#define LIBLORA_RF95_REG_HOP_CHANNEL          0x1C
#define LIBLORA_RF95_REG_MODEM_CONFIG1        0x1D
#define LIBLORA_RF95_REG_MODEM_CONFIG2        0x1E
#define LIBLORA_RF95_REG_SYMB_TIMEOUT_LSB     0x1F
#define LIBLORA_RF95_REG_PREAMBLE_MSB         0x20
#define LIBLORA_RF95_REG_PREAMBLE_LSB         0x21
#define LIBLORA_RF95_REG_PAYLOAD_LENGTH       0x22
#define LIBLORA_RF95_REG_MAX_PAYLOAD_LENGTH   0x23
#define LIBLORA_RF95_REG_HOP_PERIOD           0x24
#define LIBLORA_RF95_REG_FIFO_RX_BYTE_ADDR    0x25
#define LIBLORA_RF95_REG_MODEM_CONFIG3        0x26
#define LIBLORA_RF95_REG_PPM_CORRECTION       0x27
#define LIBLORA_RF95_REG_FEI_MSB              0x28
#define LIBLORA_RF95_REG_FEI_MID              0x29
#define LIBLORA_RF95_REG_FEI_LSB              0x2A
#define LIBLORA_RF95_REG_RSSI_WIDEBAND        0x2C
#define LIBLORA_RF95_REG_IF_FREQ2             0x2F
#define LIBLORA_RF95_REG_IF_FREQ1             0x30
#define LIBLORA_RF95_REG_DETECT_OPTI          0x31
#define LIBLORA_RF95_REG_INVERT_IQ            0x33
#define LIBLORA_RF95_REG_HIGH_BW_OPTI1        0x36
#define LIBLORA_RF95_REG_DETECT_THRESHOLD     0x37
#define LIBLORA_RF95_REG_SYNC_WORD            0x39
#define LIBLORA_RF95_REG_HIGH_BW_OPTI2        0x3A
#define LIBLORA_RF95_REG_INVERT_IQ2           0x3B

#define LIBLORA_RF95_REG_DIO_MAPPING_1        0x40
#define LIBLORA_RF95_REG_DIO_MAPPING_2        0x41
#define LIBLORA_RF95_REG_VERSION              0x42

#ifdef __cplusplus
extern "C"
{
#endif

    int liblora_reg_sx127x_r(void* dev, uint8_t addr, uint8_t* val);
    int liblora_reg_sx127x_rb(void* dev, uint8_t addr, uint8_t* buf, int len);

    int liblora_reg_sx127x_w(void* dev, uint8_t addr, uint8_t val);
    int liblora_reg_sx127x_wb(void* dev, uint8_t addr, uint8_t* buf, int len);

#ifdef __cplusplus
}
#endif

#endif