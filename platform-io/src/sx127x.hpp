#ifndef _LIBLORA_CORE_CLASSES_SX127x_H_
#define _LIBLORA_CORE_CLASSES_SX127x_H_

#include "../lib/sx127x/radio.h"
#include "../lib/board.h"

class SX127x
{
public:
    SX127x(int dio0, int rst, int spi_dev, int spi_ss, int spi_clck_speed);
    ~SX127x();

    // Init (public)
    int init(long freq, uint8_t sf, uint8_t bw);
    
    // calbacks
    void onrx(void (*onrx)(liblora_sx127x_packet_t));
    void ontx(void (*ontx)(void));

    // opmode (public)
    void sleep();
    void idle();
    bool recv(bool continuous);
    void send(bool async);

    // config (public)
    void config_frequency(long freq);
    void config_bandwidth(uint8_t bw, bool optimize);
    void config_spreading_factor(uint8_t sf, bool optimize);
    void config_coding_rate(uint8_t cr);
    void config_invert_iq(bool enable, bool rx);
    void config_sync_word(uint8_t value);

    // packet (public)
    liblora_sx127x_packet_t packet_read();
    void packet_write(uint8_t *buffer, uint8_t len);

    // other (public)
    void afc_update();
    uint16_t valid_header_count();
    uint16_t valid_packet_count();
    int64_t frequency_error();
    uint8_t rssi();
    uint8_t random();
    liblora_sx127x_modem_status_t modem_status();

    // interrupts
    int check_irq();
    int config_interrupt(void (*callback)(void));
private:
    // config (private)
    // void config_crc(bool enable);
    // void config_header_mode(bool _explicit);
    // void config_symb_timeout(uint16_t timeout);
    // void config_preamble_len(uint16_t len);
    // void config_max_payload_len(uint8_t len);
    // void config_hop_period(uint8_t value);

    // void config_pa(bool boost, uint8_t power, uint8_t max_power);
    // void config_agc(bool enable);
    // void config_pa_ramp(uint8_t ramp);

    // void config_ocp(bool enable, uint8_t trim);

    // void config_lna(bool boost, uint8_t gain);
private:
    liblora_sx127x_radio_t m_radio;
    liblora_dev_t m_dev;
};

#endif