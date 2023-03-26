/**
 * @file sx127x.h
 * @author Lucino772
 * @date 8 Mars 2023
 */

#ifndef _LIBLORA_CORE_LIB_RADIOS_SX127x_H_
#define _LIBLORA_CORE_LIB_RADIOS_SX127x_H_

#include <stdbool.h>
#include <stdint.h>

#include "liblora/core/board.h"

/** 
 * @defgroup S127x_BW SX127x Bandwidth
 * 
 * @{
*/
/** 7.8 kHz */
#define LIBLORA_SX127x_BW_007_80              0x00
/** 10.4 kHz */
#define LIBLORA_SX127x_BW_010_40              0x01
/** 15.6 kHz */
#define LIBLORA_SX127x_BW_015_60              0x02
/** 20.8kHz */
#define LIBLORA_SX127x_BW_020_80              0x03
/** 31.25 kHz */
#define LIBLORA_SX127x_BW_031_25              0x04
/** 41.7 kHz */
#define LIBLORA_SX127x_BW_041_70              0x05
/** 62.5 kHz */
#define LIBLORA_SX127x_BW_062_50              0x06
/** 125 kHz */
#define LIBLORA_SX127x_BW_125_00              0x07
/** 250 kHz */
#define LIBLORA_SX127x_BW_250_00              0x08
/** 500 kHz */
#define LIBLORA_SX127x_BW_500_00              0x09
/** @} */

/**
 * @defgroup S127x_SF SX127x Spreading Factor
 * 
 * @{
*/
#define LIBLORA_SX127x_SF_6                   0x06
#define LIBLORA_SX127x_SF_7                   0x07
#define LIBLORA_SX127x_SF_8                   0x08
#define LIBLORA_SX127x_SF_9                   0x09
#define LIBLORA_SX127x_SF_10                  0x10
#define LIBLORA_SX127x_SF_11                  0x11
#define LIBLORA_SX127x_SF_12                  0x12
/** @} */

/**
 * @defgroup SX127x_CR SX127x Coding Rate
*/
#define LIBLORA_SX127x_CR_45                  0x01
#define LIBLORA_SX127x_CR_46                  0x02
#define LIBLORA_SX127x_CR_47                  0x03
#define LIBLORA_SX127x_CR_48                  0x04
/** @} */

/**
 * @defgroup S127x_PA_RAMP SX127x PA Ramp
 * 
 * @{
*/
#define LIBLORA_SX127x_PA_RAMP_3_4MS          0x00
#define LIBLORA_SX127x_PA_RAMP_2MS            0x01
#define LIBLORA_SX127x_PA_RAMP_1MS            0x02
#define LIBLORA_SX127x_PA_RAMP_500US          0x03
#define LIBLORA_SX127x_PA_RAMP_250US          0x04
#define LIBLORA_SX127x_PA_RAMP_125US          0x05
#define LIBLORA_SX127x_PA_RAMP_100US          0x06
#define LIBLORA_SX127x_PA_RAMP_62US           0x07
#define LIBLORA_SX127x_PA_RAMP_50US           0x08
#define LIBLORA_SX127x_PA_RAMP_40US           0x09
#define LIBLORA_SX127x_PA_RAMP_31US           0x0A
#define LIBLORA_SX127x_PA_RAMP_25US           0x0B
#define LIBLORA_SX127x_PA_RAMP_20US           0x0C
#define LIBLORA_SX127x_PA_RAMP_15US           0x0D
#define LIBLORA_SX127x_PA_RAMP_12US           0x0E
#define LIBLORA_SX127x_PA_RAMP_10US           0x0F
/** @} */

#define LIBLORA_SX127x_RADIO_INIT(dev_ptr, board_ptr)    { 7, 0, dev_ptr, board_ptr, NULL, NULL }

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct liblora_sx127x_packet_s
    {
        uint8_t buffer[256];
        uint8_t size;
        int8_t pkt_snr;
        uint8_t pkt_rssi;
        uint8_t rssi;
        uint8_t strength;
    } liblora_sx127x_packet_t;

    typedef struct liblora_sx127x_radio_s
    {
        int dio0;
        int rst;
        liblora_dev_t* dev;
        void* userdata;

        void (*onrx)(liblora_sx127x_packet_t);
        void (*ontx)(void);
    } liblora_sx127x_radio_t;

    typedef struct liblora_sx127x_opmode_s
    {
        bool lora;
        bool shared_reg;
        bool low_freq;
        uint8_t mode;
    } liblora_sx127x_opmode_t;

    typedef enum
    {
        UNKNOWN = 0x0,
        SIGNAL_DETECTED = 0x1,
        SIGNAL_SYNCED = 0x2,
        RX_ON_GOING = 0x4,
        HEADER_INFO_VALID = 0x8,
        MODEM_CLEAR = 0x10,
    } liblora_sx127x_modem_status_t;

    /** Initialize the radio with a frequency, spreading factor & bandwidth */
    int liblora_sx127x_init(liblora_sx127x_radio_t *radio, long freq, uint8_t sf, uint8_t bw);

    /** Put the radio to sleep */
    void liblora_sx127x_sleep(liblora_sx127x_radio_t *radio);
    /** Put the radio in idle mode */
    void liblora_sx127x_idle(liblora_sx127x_radio_t *radio);
    /** Put the radio in receive mode */
    bool liblora_sx127x_recv(liblora_sx127x_radio_t *radio, bool continuous);
    /** Put the radio in send mode */
    void liblora_sx127x_send(liblora_sx127x_radio_t *radio, bool async);

    liblora_sx127x_opmode_t liblora_sx127x_opmode_read(liblora_sx127x_radio_t *radio);
    void liblora_sx127x_opmode_write(liblora_sx127x_radio_t *radio, liblora_sx127x_opmode_t _opmode);

    /** Set the radio frequency */
    void liblora_sx127x_config_frequency(liblora_sx127x_radio_t *radio, long freq);
    /** Set the radio bandwidth */
    void liblora_sx127x_config_bandwidth(liblora_sx127x_radio_t *radio, uint8_t bw, bool optimize);
    /** Set the radio spreading factor */
    void liblora_sx127x_config_spreading_factor(liblora_sx127x_radio_t *radio, uint8_t sf, bool optimize);
    /** Set the readio coding rate */
    void liblora_sx127x_config_coding_rate(liblora_sx127x_radio_t *radio, uint8_t cr);
    /** Invert the radio IQ */
    void liblora_sx127x_config_invert_iq(liblora_sx127x_radio_t *radio, bool enable, bool rx);
    /** Set the radio sync word */
    void liblora_sx127x_config_sync_word(liblora_sx127x_radio_t *radio, uint8_t value);

    void liblora_sx127x_config_low_data_rate_optimization(liblora_sx127x_radio_t *radio, bool enable);
    void liblora_sx127x_config_detection_optimization(liblora_sx127x_radio_t *radio, uint8_t value);
    void liblora_sx127x_config_detection_threshold(liblora_sx127x_radio_t *radio, uint8_t value);
    void liblora_sx127x_config_high_bw_optimization(liblora_sx127x_radio_t *radio, bool enable);
    
    /** Enable CRC on the radio */
    void liblora_sx127x_config_crc(liblora_sx127x_radio_t *radio, bool enable);
    /** Set the radio header mode */
    void liblora_sx127x_config_header_mode(liblora_sx127x_radio_t *radio, bool _explicit);
    /** Set the radio symbols timeout */
    void liblora_sx127x_config_symb_timeout(liblora_sx127x_radio_t *radio, uint16_t timeout);
    /** Set the radio preamble len */
    void liblora_sx127x_config_preamble_len(liblora_sx127x_radio_t *radio, uint16_t len);
    /** Set the radio maximum payload length */
    void liblora_sx127x_config_max_payload_len(liblora_sx127x_radio_t *radio, uint8_t len);
    /** Set the radio channel hopping period */
    void liblora_sx127x_config_hop_period(liblora_sx127x_radio_t *radio, uint8_t value);

    /** Set the radio PA config */
    void liblora_sx127x_config_pa(liblora_sx127x_radio_t *radio, bool boost, uint8_t power, uint8_t max_power);
    /** Enable AGC on the radio */
    void liblora_sx127x_config_agc(liblora_sx127x_radio_t *radio, bool enable);
    /** Set the radio PA ramp*/
    void liblora_sx127x_config_pa_ramp(liblora_sx127x_radio_t *radio, uint8_t ramp);

    /** Enable OCP on the radio */
    void liblora_sx127x_config_ocp(liblora_sx127x_radio_t *radio, bool enable, uint8_t trim);

    /** Set the radio LNA gain */
    void liblora_sx127x_config_lna(liblora_sx127x_radio_t *radio, bool boost, uint8_t gain);

    void liblora_sx127x_fifo_write(liblora_sx127x_radio_t *radio, uint8_t *buffer, uint8_t len);
    void liblora_sx127x_fifo_read(liblora_sx127x_radio_t *radio, uint8_t *buffer, uint8_t len);

    /** Read a packet from the radio buffer */
    liblora_sx127x_packet_t liblora_sx127x_packet_read(liblora_sx127x_radio_t *radio);
    /** Write a packet to the radio buffer */
    void liblora_sx127x_packet_write(liblora_sx127x_radio_t *radio, uint8_t *buffer, uint8_t len);

    uint8_t liblora_sx127x_packet_size(liblora_sx127x_radio_t *radio);
    uint8_t liblora_sx127x_packet_rssi(liblora_sx127x_radio_t *radio);
    int8_t liblora_sx127x_packet_snr(liblora_sx127x_radio_t *radio);
    uint8_t liblora_sx127x_packet_strength(liblora_sx127x_radio_t *radio);

    void liblora_sx127x_afc_update(liblora_sx127x_radio_t *radio);
    uint16_t liblora_sx127x_valid_header_count(liblora_sx127x_radio_t *radio);
    uint16_t liblora_sx127x_valid_packet_count(liblora_sx127x_radio_t *radio);
    int64_t liblora_sx127x_frequency_error(liblora_sx127x_radio_t *radio);
    uint8_t liblora_sx127x_rssi(liblora_sx127x_radio_t *radio);
    uint8_t liblora_sx127x_random(liblora_sx127x_radio_t *radio);
    liblora_sx127x_modem_status_t liblora_sx127x_modem_status(liblora_sx127x_radio_t *radio);

    int liblora_sx127x_check_irq(liblora_sx127x_radio_t *radio);
    int liblora_sx127x_config_interrupt(liblora_sx127x_radio_t *radio, void (*callback)(void));

#ifdef __cplusplus
}
#endif

#endif