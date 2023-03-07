#ifndef _LIBLORA_CORE_LIB_SX127x_REG_H_
#define _LIBLORA_CORE_LIB_SX127x_REG_H_

#include <stdint.h>
#include <stdbool.h>
#include "../board.h"

// Access Mode
#define SX127x_REG_FSK 1
#define SX127x_REG_LORA 2
#define SX127x_REG_SHARED (SX127x_REG_FSK | SX127x_REG_LORA)

// Register Mode
#define SX127x_REG_MODE_R 1
#define SX127x_REG_MODE_W 2
#define SX127x_REG_MODE_C 4
#define SX127x_REG_MODE_T 8

#define SX127x_REG_MODE_RW (SX127x_REG_MODE_R | SX127x_REG_MODE_W)
#define SX127x_REG_MODE_RC (SX127x_REG_MODE_R | SX127x_REG_MODE_C)
#define SX127x_REG_MODE_WT (SX127x_REG_MODE_W | SX127x_REG_MODE_T)
#define SX127x_REG_MODE_RWT (SX127x_REG_MODE_R | SX127x_REG_MODE_W | SX127x_REG_MODE_T)

#define SX127x_R(access, addr, offset, len, mode, sign) \
    ((uint32_t)access << 20) |                          \
        ((uint32_t)addr << 12) |                        \
        ((uint32_t)offset << 9) |                       \
        ((uint32_t)len << 5) |                          \
        ((uint32_t)mode << 1) |                         \
        (uint32_t)sign

#define SX127x_REG_ACCESS(val) (val >> 20) && 0x03
#define SX127x_REG_ADDR(val) (val >> 12) && 0xFF
#define SX127x_REG_OFFSET(val) (val >> 9) && 0x07
#define SX127x_REG_LEN(val) (val >> 5) && 0x0F
#define SX127x_REG_MODE(val) (val >> 1) && 0x0F
#define SX127x_REG_SIGN(val) val && 0x01

typedef struct liblora_sx127x_reg_s
{
    uint8_t access;
    uint8_t addr;
    uint8_t offset;
    uint8_t len;
    uint8_t mode;
    bool sign;
} liblora_sx127x_reg_t;

#define SX127x_REG_PARSE(val)       \
    {                               \
        SX127x_REG_ACCESS(val),     \
            SX127x_REG_ADDR(val),   \
            SX127x_REG_OFFSET(val), \
            SX127x_REG_LEN(val),    \
            SX127x_REG_MODE(val),   \
            SX127x_REG_SIGN(val)    \
    }

/* Common registers to mode FSK & LoRa */
#define LIBLORA_SX127x_REG_FIFO SX127x_R(SX127x_REG_SHARED, 0x00, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_OPMODE_DEV_MODE SX127x_R(SX127x_REG_SHARED, 0x01, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_OPMODE_LF_MODE SX127x_R(SX127x_REG_SHARED, 0x01, 3, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_OPMODE_LR_MODE SX127x_R(SX127x_REG_SHARED, 0x01, 7, 1, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FRF_MSB SX127x_R(SX127x_REG_SHARED, 0x06, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FRF_MID SX127x_R(SX127x_REG_SHARED, 0x07, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FRF_LSB SX127x_R(SX127x_REG_SHARED, 0x08, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_PA_OUTPUT_POW SX127x_R(SX127x_REG_SHARED, 0x09, 0, 4, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_PA_MAX_POW SX127x_R(SX127x_REG_SHARED, 0x09, 4, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_PA_SELECT SX127x_R(SX127x_REG_SHARED, 0x09, 7, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_PA_RAMP SX127x_R(SX127x_REG_SHARED, 0x0A, 0, 4, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_OCP_TRIM SX127x_R(SX127x_REG_SHARED, 0x0B, 0, 5, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_OCP_ON SX127x_R(SX127x_REG_SHARED, 0x0B, 5, 1, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_LNA_BOOST_HF SX127x_R(SX127x_REG_SHARED, 0x0C, 0, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LNA_BOOST_LF SX127x_R(SX127x_REG_SHARED, 0x0C, 3, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LNA_GAIN SX127x_R(SX127x_REG_SHARED, 0x0C, 5, 3, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_DIO_MAPPING1 SX127x_R(SX127x_REG_SHARED, 0x40, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_DIO_MAPPING2 SX127x_R(SX127x_REG_SHARED, 0x41, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_VERSION SX127x_R(SX127x_REG_SHARED, 0x42, 0, 8, SX127x_REG_MODE_R, 0)

#define LIBLORA_SX127x_REG_TXCO_INPUT_ON SX127x_R(SX127x_REG_SHARED, 0x4B, 4, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_PA_DAC SX127x_R(SX127x_REG_SHARED, 0x4D, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FORMER_TEMP SX127x_R(SX127x_REG_SHARED, 0x5B, 0, 8, SX127x_REG_MODE_R, 0)

#define LIBLORA_SX127x_REG_AGC_REF_LVL SX127x_R(SX127x_REG_SHARED, 0x61, 0, 6, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_AGC_THRESH1 SX127x_R(SX127x_REG_SHARED, 0x62, 0, 5, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_AGC_THRESH2 SX127x_R(SX127x_REG_SHARED, 0x63, 4, 4, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_AGC_THRESH3 SX127x_R(SX127x_REG_SHARED, 0x63, 0, 4, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_AGC_THRESH4 SX127x_R(SX127x_REG_SHARED, 0x64, 4, 4, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_AGC_THRESH5 SX127x_R(SX127x_REG_SHARED, 0x64, 0, 4, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_PLL_BW SX127x_R(SX127x_REG_SHARED, 0x70, 6, 2, SX127x_REG_MODE_RW, 0)

/* Registers specific to LoRa */
#define LIBLORA_SX127x_REG_LORA_OPMODE_SHARED_ACCESS SX127x_R(SX127x_REG_LORA, 0x01, 6, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_FIFO_ADDR_PTR SX127x_R(SX127x_REG_LORA, 0x0D, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_FIFO_TX_BASE_ADDR SX127x_R(SX127x_REG_LORA, 0x0E, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_FIFO_RX_BASE_ADDR SX127x_R(SX127x_REG_LORA, 0x0F, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_FIFO_CURR_ADDR SX127x_R(SX127x_REG_LORA, 0x10, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_LORA_IRQ_FLAGS_MASK SX127x_R(SX127x_REG_LORA, 0x11, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_IRQ_FLAGS SX127x_R(SX127x_REG_LORA, 0x12, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_LORA_FIFO_RX_BYTES_NB SX127x_R(SX127x_REG_LORA, 0x13, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_RX_HEADER_CNT_MSB SX127x_R(SX127x_REG_LORA, 0x14, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_RX_HEADER_CNT_LSB SX127x_R(SX127x_REG_LORA, 0x15, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_RX_PKT_CNT_MSB SX127x_R(SX127x_REG_LORA, 0x16, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_RX_PKT_CNT_LSB SX127x_R(SX127x_REG_LORA, 0x17, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_MODEM_STATUS SX127x_R(SX127x_REG_LORA, 0x18, 0, 5, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_RX_CODING_RATE SX127x_R(SX127x_REG_LORA, 0x18, 5, 3, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_PKT_SNR SX127x_R(SX127x_REG_LORA, 0x19, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_PKT_RSSI SX127x_R(SX127x_REG_LORA, 0x1A, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_RSSI SX127x_R(SX127x_REG_LORA, 0x1B, 0, 8, SX127x_REG_MODE_R, 0)

#define LIBLORA_SX127x_REG_LORA_HOP_CHANNEL_CURR_FHSS SX127x_R(SX127x_REG_LORA, 0x1C, 0, 6, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_CRC_ON SX127x_R(SX127x_REG_LORA, 0x1C, 6, 1, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_PLL_TIMEOUT SX127x_R(SX127x_REG_LORA, 0x1C, 7, 1, SX127x_REG_MODE_R, 0)

#define LIBLORA_SX127x_REG_LORA_MOD_CFG1_HEADER_MODE SX127x_R(SX127x_REG_LORA, 0x1D, 0, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_MOD_CFG1_CR SX127x_R(SX127x_REG_LORA, 0x1D, 1, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_MOD_CFG1_BW SX127x_R(SX127x_REG_LORA, 0x1D, 4, 4, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_LORA_MOD_CFG2_SYMB_TMOUT_MSB SX127x_R(SX127x_REG_LORA, 0x1E, 0, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_MOD_CFG2_RX_PAYLOAD_CRC SX127x_R(SX127x_REG_LORA, 0x1E, 2, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_MOD_CFG2_TX_CONT_MODE SX127x_R(SX127x_REG_LORA, 0x1E, 3, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_MOD_CFG2_SF SX127x_R(SX127x_REG_LORA, 0x1E, 4, 4, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_MOD_SYMB_TMOUT_LSB SX127x_R(SX127x_REG_LORA, 0x1F, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_LORA_PREAMBLE_LEN_MSB SX127x_R(SX127x_REG_LORA, 0x20, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_PREAMBLE_LEN_LSB SX127x_R(SX127x_REG_LORA, 0x21, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_PAYLOAD_LEN SX127x_R(SX127x_REG_LORA, 0x22, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_MAX_PAYLOAD_LEN SX127x_R(SX127x_REG_LORA, 0x23, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_FREQ_HOP_PERDIOD SX127x_R(SX127x_REG_LORA, 0x24, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_FIFO_RX_BYTE_ADDR_PTR SX127x_R(SX127x_REG_LORA, 0x25, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_LORA_CFG3_AGC_ON SX127x_R(SX127x_REG_LORA, 0x26, 2, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_CFG3_LORA_LOW_DR_OPTI SX127x_R(SX127x_REG_LORA, 0x26, 3, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_PPM_CORR SX127x_R(SX127x_REG_LORA, 0x27, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_LORA_FEI_MSB SX127x_R(SX127x_REG_LORA, 0x28, 0, 4, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_FEI_MID SX127x_R(SX127x_REG_LORA, 0x29, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_FEI_LSB SX127x_R(SX127x_REG_LORA, 0x2A, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_RSSI_WIDEBAND SX127x_R(SX127x_REG_LORA, 0x2C, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_LORA_IF_FREQ2 SX127x_R(SX127x_REG_LORA, 0x2F, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_IF_FREQ1 SX127x_R(SX127x_REG_LORA, 0x30, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_LORA_DETECT_OPTI SX127x_R(SX127x_REG_LORA, 0x31, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_AUTOMATIC_IF_ON SX127x_R(SX127x_REG_LORA, 0x31, 7, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_INVERT_IQ_TX SX127x_R(SX127x_REG_LORA, 0x33, 0, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_INVERT_IQ_RX SX127x_R(SX127x_REG_LORA, 0x33, 6, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_HIGH_BW_OPTI1 SX127x_R(SX127x_REG_LORA, 0x36, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_DETECT_THRESH SX127x_R(SX127x_REG_LORA, 0x37, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_SYNC_WORD SX127x_R(SX127x_REG_LORA, 0x39, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_HIGH_BW_OPTI2 SX127x_R(SX127x_REG_LORA, 0x3A, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_LORA_INVERT_IQ2 SX127x_R(SX127x_REG_LORA, 0x3B, 0, 8, SX127x_REG_MODE_RW, 0)

/* Registers specific to FSK */
#define LIBLORA_SX127x_REG_FSK_MODE_TYPE SX127x_R(SX127x_REG_FSK, 0x01, 5, 2, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_BITRATE_MSB SX127x_R(SX127x_REG_FSK, 0x02, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_BITRATE_LSB SX127x_R(SX127x_REG_FSK, 0x03, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_FDEV_MSB SX127x_R(SX127x_REG_FSK, 0x04, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_FDEV_LSB SX127x_R(SX127x_REG_FSK, 0x05, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_PA_MOD_SHAPING SX127x_R(SX127x_REG_FSK, 0x0A, 5, 2, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_RX_CFG_TRIGGER SX127x_R(SX127x_REG_FSK, 0x0D, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RX_AGC_ON SX127x_R(SX127x_REG_FSK, 0x0D, 3, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RX_AFC_ON SX127x_R(SX127x_REG_FSK, 0x0D, 4, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RX_RX_RESTART1 SX127x_R(SX127x_REG_FSK, 0x0D, 5, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RX_RX_RESTART2 SX127x_R(SX127x_REG_FSK, 0x0D, 6, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RX_RX_RESTART3 SX127x_R(SX127x_REG_FSK, 0x0D, 7, 1, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_RSSI_CFG_SMOOTHING SX127x_R(SX127x_REG_FSK, 0x0E, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RSSI_CFG_OFFSET SX127x_R(SX127x_REG_FSK, 0x0E, 3, 5, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RSSI_COLL_THRESH SX127x_R(SX127x_REG_FSK, 0x0F, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RSSI_THRESH SX127x_R(SX127x_REG_FSK, 0x10, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RSSI_VAL SX127x_R(SX127x_REG_FSK, 0x11, 0, 8, SX127x_REG_MODE_R, 0)

#define LIBLORA_SX127x_REG_FSK_RX_BW_EXP SX127x_R(SX127x_REG_FSK, 0x12, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RX_BW_MANT SX127x_R(SX127x_REG_FSK, 0x12, 3, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RX_BW_EXP_AFC SX127x_R(SX127x_REG_FSK, 0x13, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RX_BW_MANT_AFC SX127x_R(SX127x_REG_FSK, 0x13, 3, 2, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_OOK_PEAK_THRESH_STEP SX127x_R(SX127x_REG_FSK, 0x14, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_OOK_PEAK_THRESH_TYPE SX127x_R(SX127x_REG_FSK, 0x14, 3, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_OOK_PEAK_BIT_SYNC_ON SX127x_R(SX127x_REG_FSK, 0x14, 5, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_OOK_FIXED_THRESH SX127x_R(SX127x_REG_FSK, 0x15, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_OOK_AVG_THRESH_FIT SX127x_R(SX127x_REG_FSK, 0x16, 0, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_OOK_AVG_OFFSET SX127x_R(SX127x_REG_FSK, 0x16, 2, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_OOK_PEAK_THRESH_DEC SX127x_R(SX127x_REG_FSK, 0x16, 5, 3, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_AFC_FEI SX127x_R(SX127x_REG_FSK, 0x1A, 0, 5, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_AFC_MSB SX127x_R(SX127x_REG_FSK, 0x1B, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_AFC_LSB SX127x_R(SX127x_REG_FSK, 0x1C, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_FEI_MSB SX127x_R(SX127x_REG_FSK, 0x1D, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_FEI_LSB SX127x_R(SX127x_REG_FSK, 0x1E, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_PREAMBLE_DETECT_TOL SX127x_R(SX127x_REG_FSK, 0x1F, 0, 5, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PREAMBLE_DETECT_SIZE SX127x_R(SX127x_REG_FSK, 0x1F, 7, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PREAMBLE_DETECT_ON SX127x_R(SX127x_REG_FSK, 0x1F, 7, 1, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_RX_TMOUT1 SX127x_R(SX127x_REG_FSK, 0x20, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RX_TMOUT2 SX127x_R(SX127x_REG_FSK, 0x21, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_RX_TMOUT3 SX127x_R(SX127x_REG_FSK, 0x22, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PKT_RX_DELAY SX127x_R(SX127x_REG_FSK, 0x23, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_OSC_CLK_OUT SX127x_R(SX127x_REG_FSK, 0x24, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_OSC_CAL_START SX127x_R(SX127x_REG_FSK, 0x24, 3, 1, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_PREAMBLE_LEN_MSB SX127x_R(SX127x_REG_FSK, 0x25, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PREAMBLE_LEN_LSB SX127x_R(SX127x_REG_FSK, 0x26, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_SYNC_CFG_SIZE SX127x_R(SX127x_REG_FSK, 0x27, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_SYNC_CFG_ON SX127x_R(SX127x_REG_FSK, 0x27, 4, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_SYNC_CFG_PREAMBLE_POL SX127x_R(SX127x_REG_FSK, 0x27, 5, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_SYNC_CFG_RESTART_RX SX127x_R(SX127x_REG_FSK, 0x27, 6, 2, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_SYNC_VAL1 SX127x_R(SX127x_REG_FSK, 0x28, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_SYNC_VAL2 SX127x_R(SX127x_REG_FSK, 0x29, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_SYNC_VAL3 SX127x_R(SX127x_REG_FSK, 0x2A, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_SYNC_VAL4 SX127x_R(SX127x_REG_FSK, 0x2B, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_SYNC_VAL5 SX127x_R(SX127x_REG_FSK, 0x2C, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_SYNC_VAL6 SX127x_R(SX127x_REG_FSK, 0x2D, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_SYNC_VAL7 SX127x_R(SX127x_REG_FSK, 0x2E, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_SYNC_VAL8 SX127x_R(SX127x_REG_FSK, 0x2F, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_PKT_CFG1_CRC_WHITENING SX127x_R(SX127x_REG_FSK, 0x30, 0, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PKT_CFG1_ADDR_FILTER SX127x_R(SX127x_REG_FSK, 0x30, 1, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PKT_CFG1_CRC_AUTO_CLR SX127x_R(SX127x_REG_FSK, 0x30, 3, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PKT_CFG1_CRC_ON SX127x_R(SX127x_REG_FSK, 0x30, 4, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PKT_CFG1_DC_FREE SX127x_R(SX127x_REG_FSK, 0x30, 5, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PKT_CFG1_PKT_FMT SX127x_R(SX127x_REG_FSK, 0x30, 7, 1, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_PKT_CFG2_PAYLOAD_LEN_MSB SX127x_R(SX127x_REG_FSK, 0x31, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PKT_CFG2_BEACON_ON SX127x_R(SX127x_REG_FSK, 0x31, 3, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PKT_CFG2_IO_HOME_ON SX127x_R(SX127x_REG_FSK, 0x31, 5, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PKT_CFG2_DATA_MODE SX127x_R(SX127x_REG_FSK, 0x31, 6, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_PAYLOAD_LEN_LSB SX127x_R(SX127x_REG_FSK, 0x32, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_NODE_ADDR SX127x_R(SX127x_REG_FSK, 0x33, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_BOARDCAST_ADDR SX127x_R(SX127x_REG_FSK, 0x34, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_FIFO_THRESH SX127x_R(SX127x_REG_FSK, 0x35, 0, 6, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_FIFO_TX_START_COND SX127x_R(SX127x_REG_FSK, 0x35, 7, 1, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_SEQ_CFG1_FROM_TX SX127x_R(SX127x_REG_FSK, 0x36, 0, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_SEQ_CFG1_FROM_IDLE SX127x_R(SX127x_REG_FSK, 0x36, 1, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_SEQ_CFG1_LOW_POW_SEL SX127x_R(SX127x_REG_FSK, 0x36, 2, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_SEQ_CFG1_FROM_START SX127x_R(SX127x_REG_FSK, 0x36, 3, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_SEQ_CFG1_IDLE_MODE SX127x_R(SX127x_REG_FSK, 0x36, 5, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_SEQ_CFG1_SEQ_STOP SX127x_R(SX127x_REG_FSK, 0x36, 6, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_SEQ_CFG1_SEQ_START SX127x_R(SX127x_REG_FSK, 0x36, 7, 1, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_SEQ_CFG2_FROM_PKT_RECV SX127x_R(SX127x_REG_FSK, 0x37, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_SEQ_CFG2_FROM_TX_TMOUT SX127x_R(SX127x_REG_FSK, 0x37, 3, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_SEQ_CFG2_FROM_RECV SX127x_R(SX127x_REG_FSK, 0x37, 5, 3, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_TIMER2_RESOL SX127x_R(SX127x_REG_FSK, 0x38, 0, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_TIMER1_RESOL SX127x_R(SX127x_REG_FSK, 0x38, 2, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_TIMER1_COEF SX127x_R(SX127x_REG_FSK, 0x39, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_TIMER2_COEF SX127x_R(SX127x_REG_FSK, 0x3A, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_IMG_CAL_TEMP_MONITOR SX127x_R(SX127x_REG_FSK, 0x3B, 0, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_IMG_CAL_TEMP_THRESH SX127x_R(SX127x_REG_FSK, 0x3B, 1, 2, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_IMG_CAL_TEMP_CHANGE SX127x_R(SX127x_REG_FSK, 0x3B, 3, 1, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_FSK_IMG_CAL_RUNNING SX127x_R(SX127x_REG_FSK, 0x3B, 5, 1, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_FSK_IMG_CAL_START SX127x_R(SX127x_REG_FSK, 0x3B, 6, 1, SX127x_REG_MODE_WT, 0)
#define LIBLORA_SX127x_REG_FSK_IMG_CAL_AUTO_ON SX127x_R(SX127x_REG_FSK, 0x3B, 7, 1, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_TEMP_VAL SX127x_R(SX127x_REG_FSK, 0x3C, 0, 8, SX127x_REG_MODE_R, 0)
#define LIBLORA_SX127x_REG_FSK_LOW_BAT_TRIM SX127x_R(SX127x_REG_FSK, 0x3D, 0, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_LOW_BAT_ON SX127x_R(SX127x_REG_FSK, 0x3D, 3, 1, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_IRQ_FLAGS1 SX127x_R(SX127x_REG_FSK, 0x3E, 0, 8, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_IRQ_FLAGS2 SX127x_R(SX127x_REG_FSK, 0x3F, 0, 8, SX127x_REG_MODE_RW, 0)

#define LIBLORA_SX127x_REG_FSK_PLL_FAST_HOP_ON SX127x_R(SX127x_REG_FSK, 0x44, 5, 3, SX127x_REG_MODE_RW, 0)
#define LIBLORA_SX127x_REG_FSK_BITRATE_FRAC SX127x_R(SX127x_REG_FSK, 0x5D, 0, 4, SX127x_REG_MODE_RW, 0)

#ifdef __cplusplus
extern "C"
{
#endif

    int liblora_sx127x_reg_read(liblora_dev_t *dev, uint32_t reg, uint8_t *val, void* userdata);
    int liblora_sx127x_reg_readb(liblora_dev_t *dev, uint32_t reg, uint8_t *buff, int len, void* userdata);
    int liblora_sx127x_reg_write(liblora_dev_t *dev, uint32_t reg, uint8_t val, void* userdata);
    int liblora_sx127x_reg_writeb(liblora_dev_t *dev, uint32_t reg, uint8_t *buff, int len, void* userdata);

#ifdef __cplusplus
}
#endif

#endif