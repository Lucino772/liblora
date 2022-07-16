#ifndef _LIBLORA_SX127x_DRIVER_H_
#define _LIBLORA_SX127x_DRIVER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include "./config.h"

    // structs
    typedef struct
    {
        int spi_dev;
        int spi_ss;
        int dio0;
        int rst;
    } sx127x_dev_t;

    typedef struct
    {
        bool lora = false;
        bool shared_reg = false;
        bool low_freq = false;
        uint8_t mode = SX127x_OPMODE_SLEEP;
    } sx127x_opmode_t;

    typedef struct
    {
        uint8_t buffer[256];
        uint8_t size;
        int8_t pkt_snr;
        uint8_t pkt_rssi;
        uint8_t rssi;
        uint8_t strength;
    } sx127x_packet_t;

    typedef enum
    {
        UNKNOWN = 0x0,
        SIGNAL_DETECTED = 0x1,
        SIGNAL_SYNCED = 0x2,
        RX_ON_GOING = 0x4,
        HEADER_INFO_VALID = 0x8,
        MODEM_CLEAR = 0x10,
    } sx127x_modem_status_t;

    // Initialisation
    sx127x_dev_t sx127x_create(int spi_dev, int spi_ss, int dio0, int rst);
    int sx127x_init(sx127x_dev_t dev, long freq, uint8_t sf, uint8_t bw);

    // opmode (private)
    sx127x_opmode_t sx127x_opmode_read(sx127x_dev_t dev);
    void sx127x_opmode_write(sx127x_dev_t dev, sx127x_opmode_t _opmode);

    // opmode (public)
    void sx127x_sleep(sx127x_dev_t dev);
    void sx127x_idle(sx127x_dev_t dev);

    bool sx127x_receive(sx127x_dev_t dev, bool continuous);
    sx127x_packet_t sx127x_get_packet(sx127x_dev_t dev);

    void sx127x_send(sx127x_dev_t dev, uint8_t *buffer, uint8_t len, bool async);

    // config (public)
    void sx127x_config_frequency(sx127x_dev_t dev, long freq);
    void sx127x_config_bandwidth(sx127x_dev_t dev, uint8_t bw, bool optimize);
    void sx127x_config_spreading_factor(sx127x_dev_t dev, uint8_t sf, bool optimize);
    void sx127x_config_coding_rate(sx127x_dev_t dev, uint8_t cr);
    void sx127x_config_invert_iq(sx127x_dev_t dev, bool enable, bool rx);
    void sx127x_config_sync_word(sx127x_dev_t dev, uint8_t value);

    // config/optimization (private)
    void sx127x_config_low_data_rate_optimization(sx127x_dev_t dev, bool enable);
    void sx127x_config_detection_optimization(sx127x_dev_t dev, uint8_t value);
    void sx127x_config_detection_threshold(sx127x_dev_t dev, uint8_t value);
    void sx127x_config_high_bw_optimization(sx127x_dev_t dev, bool enable);

    // config (private)
    void sx127x_config_crc(sx127x_dev_t dev, bool enable);
    void sx127x_config_header_mode(sx127x_dev_t dev, bool _explicit);
    void sx127x_config_symb_timeout(sx127x_dev_t dev, uint16_t timeout);
    void sx127x_config_preamble_len(sx127x_dev_t dev, uint16_t len);
    void sx127x_config_max_payload_len(sx127x_dev_t dev, uint8_t len);
    void sx127x_config_hop_period(sx127x_dev_t dev, uint8_t value);

    void sx127x_config_pa(sx127x_dev_t dev, bool boost, uint8_t power, uint8_t max_power);
    void sx127x_config_agc(sx127x_dev_t dev, bool enable);
    void sx127x_config_pa_ramp(sx127x_dev_t dev, uint8_t ramp);

    void sx127x_config_ocp(sx127x_dev_t dev, bool enable, uint8_t trim);

    void sx127x_config_lna(sx127x_dev_t dev, bool boost, uint8_t gain);

    // fifo (private)
    void sx127x_fifo_write(sx127x_dev_t dev, uint8_t *buffer, uint8_t len);
    void sx127x_fifo_read(sx127x_dev_t dev, uint8_t *buffer, uint8_t len);

    // packet (private)
    uint8_t sx127x_packet_size(sx127x_dev_t dev);
    uint8_t sx127x_packet_rssi(sx127x_dev_t dev);
    int8_t sx127x_packet_snr(sx127x_dev_t dev);
    uint8_t sx127x_packet_strength(sx127x_dev_t dev);

    sx127x_packet_t sx127x_read_packet(sx127x_dev_t dev);
    void sx127x_write_packet(sx127x_dev_t dev, uint8_t *buffer, uint8_t len);

    // other (public)
    uint16_t sx127x_valid_header_count(sx127x_dev_t dev);
    uint16_t sx127x_valid_packet_count(sx127x_dev_t dev);
    int64_t sx127x_frequency_error(sx127x_dev_t dev);
    uint8_t sx127x_rssi(sx127x_dev_t dev);
    uint8_t sx127x_random(sx127x_dev_t dev);
    sx127x_modem_status_t sx127x_modem_status(sx127x_dev_t dev);

#ifdef __cplusplus
}
#endif

#endif