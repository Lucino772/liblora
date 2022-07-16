#ifndef _LIBLORA_SX127x_REGISTERS_H_
#define _LIBLORA_SX127x_REGISTERS_H_

/****************** FIFO ******************/
#define SX127x_REG_FIFO                 0x00

/**************** Settings ****************/
#define SX127x_REG_OPMODE               0x01
#define SX127x_REG_FR_MSB               0x06
#define SX127x_REG_FR_MID               0x07
#define SX127x_REG_FR_LSB               0x08

/**************** RF Block ****************/
#define SX127x_REG_PA_CONFIG            0x09
#define SX127x_REG_PA_RAMP              0x0A
#define SX127x_REG_OCP                  0x0B
#define SX127x_REG_LNA                  0x0C

/*************** Lora Page ****************/
#define SX127x_REG_FIFO_ADDR_PTR        0x0D
#define SX127x_REG_FIFO_TX_BASE_ADDR    0x0E
#define SX127x_REG_FIFO_RX_BASE_ADDR    0x0F
#define SX127x_REG_FIFO_RX_CURR_ADDR    0x10

#define SX127x_REG_IRQ_FLAGS_MASK       0x11
#define SX127x_REG_IRQ_FLAGS            0x12

#define SX127x_REG_RX_NB_BYTES          0x13
#define SX127x_REG_RX_HEAD_CNT_VAL_MSB  0x14
#define SX127x_REG_RX_HEAD_CNT_VAL_LSB  0x15
#define SX127x_REG_RX_PKT_CNT_VAL_MSB   0x16
#define SX127x_REG_RX_PKT_CNT_VAL_LSB   0x17

#define SX127x_REG_MODEM_STATUS         0x18
#define SX127x_REG_PKT_SNR_VAL          0x19
#define SX127x_REG_PKT_RSSI_VAL         0x1A
#define SX127x_REG_RSSI_VAL             0x1B

#define SX127x_REG_HOP_CHANNEL          0x1C
#define SX127x_REG_MODEM_CONFIG1        0x1D
#define SX127x_REG_MODEM_CONFIG2        0x1E
#define SX127x_REG_SYMB_TIMEOUT_LSB     0x1F
#define SX127x_REG_PREAMBLE_MSB         0x20
#define SX127x_REG_PREAMBLE_LSB         0x21
#define SX127x_REG_PAYLOAD_LENGTH       0x22
#define SX127x_REG_MAX_PAYLOAD_LENGTH   0x23
#define SX127x_REG_HOP_PERIOD           0x24
#define SX127x_REG_FIFO_RX_BYTE_ADDR    0x25
#define SX127x_REG_MODEM_CONFIG3        0x26
#define SX127x_REG_PPM_CORRECTION       0x27
#define SX127x_REG_FEI_MSB              0x28
#define SX127x_REG_FEI_MID              0x29
#define SX127x_REG_FEI_LSB              0x2A
#define SX127x_REG_RSSI_WIDEBAND        0x2C
#define SX127x_REG_IF_FREQ2             0x2F
#define SX127x_REG_IF_FREQ1             0x30
#define SX127x_REG_DETECT_OPTI          0x31
#define SX127x_REG_INVERT_IQ            0x33
#define SX127x_REG_HIGH_BW_OPTI1        0x36
#define SX127x_REG_DETECT_THRESHOLD     0x37
#define SX127x_REG_SYNC_WORD	        0x39
#define SX127x_REG_HIGH_BW_OPTI2        0x3A
#define SX127x_REG_INVERT_IQ2           0x3B

/***************** Other ******************/
#define SX127x_REG_DIO_MAPPING_1        0x40
#define SX127x_REG_DIO_MAPPING_2        0x41
#define SX127x_REG_VERSION	  			0x42

#endif