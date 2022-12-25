/*
 * Created on Fri Aug 12 2022
 *
 * Copyright (c) 2022 Lucino772
 */

#ifndef _LIBLORA_PRIVATE_RF95_H_
#define _LIBLORA_PRIVATE_RF95_H_

#include <stdio.h>
#include "drivers/_drivers.h"
#include "../include/liblora/rf95.h"

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
#define LIBLORA_RF95_REG_SYNC_WORD	        0x39
#define LIBLORA_RF95_REG_HIGH_BW_OPTI2        0x3A
#define LIBLORA_RF95_REG_INVERT_IQ2           0x3B

#define LIBLORA_RF95_REG_DIO_MAPPING_1        0x40
#define LIBLORA_RF95_REG_DIO_MAPPING_2        0x41
#define LIBLORA_RF95_REG_VERSION              0x42

/**============================================
 *               OPMODE
 *=============================================**/
#define LIBLORA_RF95_OPMODE_SLEEP             0x00
#define LIBLORA_RF95_OPMODE_STDBY             0x01
#define LIBLORA_RF95_OPMODE_FSTX              0x02
#define LIBLORA_RF95_OPMODE_TX                0x03
#define LIBLORA_RF95_OPMODE_FSRX              0x04
#define LIBLORA_RF95_OPMODE_RX_CONT           0x05
#define LIBLORA_RF95_OPMODE_RX_SINGLE         0x06
#define LIBLORA_RF95_OPMODE_CAD               0x07

/**============================================
 *               IRQ (MASK)
 *=============================================**/
#define LIBLORA_RF95_IRQ_CAD_DETECTED         0x01 // -------1
#define LIBLORA_RF95_IRQ_FHSS_CHANGE_CHAN     0x02 // ------1-
#define LIBLORA_RF95_IRQ_CAD_DONE             0x04 // -----1--
#define LIBLORA_RF95_IRQ_TX_DONE              0x08 // ----1---
#define LIBLORA_RF95_IRQ_VALID_HEADER         0x10 // ---1----
#define LIBLORA_RF95_IRQ_PAYLOAD_CRC_ERR      0x20 // --1-----
#define LIBLORA_RF95_IRQ_RX_DONE              0x40 // -1------
#define LIBLORA_RF95_IRQ_RX_TIMEOUT           0x80 // 1-------

/**============================================
 *               DIO MAPPING
 *=============================================**/
#define LIBLORA_RF95_DIO0_RX_DONE             0x00 // 00------
#define LIBLORA_RF95_DIO0_TX_DONE             0x01 // 01------
#define LIBLORA_RF95_DIO0_CAD_DONE            0x02 // 10------
#define LIBLORA_RF95_DIO0_NOP                 0xC0 // 11------

#define LIBLORA_RF95_DIO1_RX_TIMEOUT          0x00 // --00----
#define LIBLORA_RF95_DIO1_FHSS_CHANGE_CHAN    0x10 // --01----
#define LIBLORA_RF95_DIO1_CAD_DETECTED        0x20 // --10----
#define LIBLORA_RF95_DIO1_NOP                 0x30 // --11----

#define LIBLORA_RF95_DIO2_FHSS_CHANGE_CHAN    0x00 // ----00--
#define LIBLORA_RF95_DIO2_NOP                 0x0C // ----11--

#define LIBLORA_RF95_DIO3_CAD_DONE            0x00 // ------00
#define LIBLORA_RF95_DIO3_VALID_HEADER        0x01 // ------01
#define LIBLORA_RF95_DIO3_PAYLOAD_CRC_ERR     0x02 // ------10
#define LIBLORA_RF95_DIO3_NOP                 0x03 // ------11


#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        bool lora;
        bool shared_reg;
        bool low_freq;
        uint8_t mode;
    } liblora_rf95_opmode_t;

    // Opmode (private)
    liblora_rf95_opmode_t liblora_rf95_opmode_read(liblora_rf95_radio_t *radio);
    void liblora_rf95_opmode_write(liblora_rf95_radio_t *radio, liblora_rf95_opmode_t _opmode);

    // config/optimization (private)
    void liblora_rf95_config_low_data_rate_optimization(liblora_rf95_radio_t *radio, bool enable);
    void liblora_rf95_config_detection_optimization(liblora_rf95_radio_t *radio, uint8_t value);
    void liblora_rf95_config_detection_threshold(liblora_rf95_radio_t *radio, uint8_t value);
    void liblora_rf95_config_high_bw_optimization(liblora_rf95_radio_t *radio, bool enable);

    // config (private)
    void liblora_rf95_config_crc(liblora_rf95_radio_t *radio, bool enable);
    void liblora_rf95_config_header_mode(liblora_rf95_radio_t *radio, bool _explicit);
    void liblora_rf95_config_symb_timeout(liblora_rf95_radio_t *radio, uint16_t timeout);
    void liblora_rf95_config_preamble_len(liblora_rf95_radio_t *radio, uint16_t len);
    void liblora_rf95_config_max_payload_len(liblora_rf95_radio_t *radio, uint8_t len);
    void liblora_rf95_config_hop_period(liblora_rf95_radio_t *radio, uint8_t value);

    void liblora_rf95_config_pa(liblora_rf95_radio_t *radio, bool boost, uint8_t power, uint8_t max_power);
    void liblora_rf95_config_agc(liblora_rf95_radio_t *radio, bool enable);
    void liblora_rf95_config_pa_ramp(liblora_rf95_radio_t *radio, uint8_t ramp);

    void liblora_rf95_config_ocp(liblora_rf95_radio_t *radio, bool enable, uint8_t trim);

    void liblora_rf95_config_lna(liblora_rf95_radio_t *radio, bool boost, uint8_t gain);

    // fifo (private)
    void liblora_rf95_fifo_write(liblora_rf95_radio_t *radio, uint8_t *buffer, uint8_t len);
    void liblora_rf95_fifo_read(liblora_rf95_radio_t *radio, uint8_t *buffer, uint8_t len);

    // packet (private)
    uint8_t liblora_rf95_packet_size(liblora_rf95_radio_t *radio);
    uint8_t liblora_rf95_packet_rssi(liblora_rf95_radio_t *radio);
    int8_t liblora_rf95_packet_snr(liblora_rf95_radio_t *radio);
    uint8_t liblora_rf95_packet_strength(liblora_rf95_radio_t *radio);

    // interrups
    static liblora_rf95_radio_t* radios[64];
    static void liblora_rf95_handle_interrupt(int pin);

#ifdef __cplusplus
}
#endif

#endif