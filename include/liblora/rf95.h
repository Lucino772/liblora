/*
 * Created on Fri Aug 12 2022
 *
 * Copyright (c) 2022 Lucino772
 */

#ifndef _LIBLORA_RF95_H_
#define _LIBLORA_RF95_H_

/**============================================
 *               CONFIG
 *=============================================**/
#define LIBLORA_RF95_BW_7_8                   0x00
#define LIBLORA_RF95_BW_10_4                  0x01
#define LIBLORA_RF95_BW_15_6                  0x02
#define LIBLORA_RF95_BW_20_8                  0x03
#define LIBLORA_RF95_BW_31_25                 0x04
#define LIBLORA_RF95_BW_41_7                  0x05
#define LIBLORA_RF95_BW_62_5                  0x06
#define LIBLORA_RF95_BW_125                   0x07
#define LIBLORA_RF95_BW_250                   0x08
#define LIBLORA_RF95_BW_500                   0x09

#define LIBLORA_RF95_SF_6                     0x06
#define LIBLORA_RF95_SF_7                     0x07
#define LIBLORA_RF95_SF_8                     0x08
#define LIBLORA_RF95_SF_9                     0x09
#define LIBLORA_RF95_SF_10                    0x10
#define LIBLORA_RF95_SF_11                    0x11
#define LIBLORA_RF95_SF_12                    0x12

#define LIBLORA_RF95_CR_45                    0x01
#define LIBLORA_RF95_CR_46                    0x02
#define LIBLORA_RF95_CR_47                    0x03
#define LIBLORA_RF95_CR_48                    0x04

#define LIBLORA_RF95_PA_RAMP_3_4MS            0x00
#define LIBLORA_RF95_PA_RAMP_2MS              0x01
#define LIBLORA_RF95_PA_RAMP_1MS              0x02
#define LIBLORA_RF95_PA_RAMP_500US            0x03
#define LIBLORA_RF95_PA_RAMP_250US            0x04
#define LIBLORA_RF95_PA_RAMP_125US            0x05
#define LIBLORA_RF95_PA_RAMP_100US            0x06
#define LIBLORA_RF95_PA_RAMP_62US             0x07
#define LIBLORA_RF95_PA_RAMP_50US             0x08
#define LIBLORA_RF95_PA_RAMP_40US             0x09
#define LIBLORA_RF95_PA_RAMP_31US             0x0A
#define LIBLORA_RF95_PA_RAMP_25US             0x0B
#define LIBLORA_RF95_PA_RAMP_20US             0x0C
#define LIBLORA_RF95_PA_RAMP_15US             0x0D
#define LIBLORA_RF95_PA_RAMP_12US             0x0E
#define LIBLORA_RF95_PA_RAMP_10US             0x0F

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    #define LIBLORA_RF95_RADIO_INIT(com) { 7, 0, com, NULL, NULL };

    typedef struct
    {
        uint8_t buffer[256];
        uint8_t size;
        int8_t pkt_snr;
        uint8_t pkt_rssi;
        uint8_t rssi;
        uint8_t strength;
    } liblora_rf95_packet_t;

    typedef struct
    {
        int dio0;
        int rst;
        void* com;

        void (*onrx)(liblora_rf95_packet_t);
        void (*ontx)(void);
    } liblora_rf95_radio_t;

    typedef struct
    {
        bool lora;
        bool shared_reg;
        bool low_freq;
        uint8_t mode;
    } liblora_rf95_opmode_t;

    typedef enum
    {
        UNKNOWN = 0x0,
        SIGNAL_DETECTED = 0x1,
        SIGNAL_SYNCED = 0x2,
        RX_ON_GOING = 0x4,
        HEADER_INFO_VALID = 0x8,
        MODEM_CLEAR = 0x10,
    } liblora_rf95_modem_status_t;

    // Init (public)
    int liblora_rf95_init(liblora_rf95_radio_t *radio, long freq, uint8_t sf, uint8_t bw);

    // opmode (public)
    void liblora_rf95_sleep(liblora_rf95_radio_t *radio);
    void liblora_rf95_idle(liblora_rf95_radio_t *radio);
    bool liblora_rf95_recv(liblora_rf95_radio_t *radio, bool continuous);
    void liblora_rf95_send(liblora_rf95_radio_t *radio, bool async);
    // Opmode (private)
    liblora_rf95_opmode_t liblora_rf95_opmode_read(liblora_rf95_radio_t *radio);
    void liblora_rf95_opmode_write(liblora_rf95_radio_t *radio, liblora_rf95_opmode_t _opmode);

    // config (public)
    void liblora_rf95_config_frequency(liblora_rf95_radio_t *radio, long freq);
    void liblora_rf95_config_bandwidth(liblora_rf95_radio_t *radio, uint8_t bw, bool optimize);
    void liblora_rf95_config_spreading_factor(liblora_rf95_radio_t *radio, uint8_t sf, bool optimize);
    void liblora_rf95_config_coding_rate(liblora_rf95_radio_t *radio, uint8_t cr);
    void liblora_rf95_config_invert_iq(liblora_rf95_radio_t *radio, bool enable, bool rx);
    void liblora_rf95_config_sync_word(liblora_rf95_radio_t *radio, uint8_t value);
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

    // packet (public)
    liblora_rf95_packet_t liblora_rf95_packet_read(liblora_rf95_radio_t *radio);
    void liblora_rf95_packet_write(liblora_rf95_radio_t *radio, uint8_t *buffer, uint8_t len);
    // packet (private)
    uint8_t liblora_rf95_packet_size(liblora_rf95_radio_t *radio);
    uint8_t liblora_rf95_packet_rssi(liblora_rf95_radio_t *radio);
    int8_t liblora_rf95_packet_snr(liblora_rf95_radio_t *radio);
    uint8_t liblora_rf95_packet_strength(liblora_rf95_radio_t *radio);

    // other (public)
    uint16_t liblora_rf95_valid_header_count(liblora_rf95_radio_t *radio);
    uint16_t liblora_rf95_valid_packet_count(liblora_rf95_radio_t *radio);
    int64_t liblora_rf95_frequency_error(liblora_rf95_radio_t *radio);
    uint8_t liblora_rf95_rssi(liblora_rf95_radio_t *radio);
    uint8_t liblora_rf95_random(liblora_rf95_radio_t *radio);
    liblora_rf95_modem_status_t liblora_rf95_modem_status(liblora_rf95_radio_t *radio);

    // interrupts
    void liblora_rf95_onrx(liblora_rf95_radio_t *radio, void (*callback)(liblora_rf95_packet_t));
    void liblora_rf95_ontx(liblora_rf95_radio_t *radio, void (*callback)(void));

#ifdef __cplusplus
}
#endif

#endif