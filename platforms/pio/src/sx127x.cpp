#include "liblora/sx127x.hpp"

SX127x::SX127x(int dio0, int rst, int spi_dev, int spi_ss, int spi_clck_speed)
{
    m_dev.spi_dev = spi_dev;
    m_dev.spi_ss = spi_ss;
    m_dev.spi_clck_speed = spi_clck_speed;

    m_radio.dio0 = dio0;
    m_radio.rst = rst;
    m_radio.dev = &m_dev;
    m_radio.userdata = nullptr;
    m_radio.onrx = nullptr;
    m_radio.ontx = nullptr;
}

SX127x::~SX127x()
{
    m_radio.dev = nullptr;
}

int SX127x::init(long freq, uint8_t sf, uint8_t bw)
{
    return liblora_sx127x_init(&m_radio, freq, sf, bw);
}

void SX127x::onrx(void (*onrx)(liblora_sx127x_packet_t))
{
    m_radio.onrx = onrx;
}

void SX127x::ontx(void (*ontx)(void))
{
    m_radio.ontx = ontx;
}


void SX127x::sleep()
{
    liblora_sx127x_sleep(&m_radio);
}

void SX127x::idle()
{
    liblora_sx127x_idle(&m_radio);
}

bool SX127x::recv(bool continuous)
{
    return liblora_sx127x_recv(&m_radio, continuous);
}

void SX127x::send(bool async)
{
    liblora_sx127x_send(&m_radio, async);
}

void SX127x::config_frequency(long freq)
{
    liblora_sx127x_config_frequency(&m_radio, freq);
}

void SX127x::config_bandwidth(uint8_t bw, bool optimize)
{
    liblora_sx127x_config_bandwidth(&m_radio, bw, optimize);
}

void SX127x::config_spreading_factor(uint8_t sf, bool optimize)
{
    liblora_sx127x_config_spreading_factor(&m_radio, sf, optimize);
}

void SX127x::config_coding_rate(uint8_t cr)
{
    liblora_sx127x_config_coding_rate(&m_radio, cr);
}

void SX127x::config_invert_iq(bool enable, bool rx)
{
    liblora_sx127x_config_invert_iq(&m_radio, enable, rx);
}

void SX127x::config_sync_word(uint8_t value)
{
    liblora_sx127x_config_sync_word(&m_radio, value);
}

liblora_sx127x_packet_t SX127x::packet_read()
{
    return liblora_sx127x_packet_read(&m_radio);
}

void SX127x::afc_update()
{
    liblora_sx127x_afc_update(&m_radio);
}

uint16_t SX127x::valid_header_count()
{
    return liblora_sx127x_valid_header_count(&m_radio);
}

uint16_t SX127x::valid_packet_count()
{
    return liblora_sx127x_valid_packet_count(&m_radio);
}

int64_t SX127x::frequency_error()
{
    return liblora_sx127x_frequency_error(&m_radio);
}

uint8_t SX127x::rssi()
{
    return liblora_sx127x_rssi(&m_radio);
}

uint8_t SX127x::random()
{
    return liblora_sx127x_random(&m_radio);
}

liblora_sx127x_modem_status_t SX127x::modem_status()
{
    return liblora_sx127x_modem_status(&m_radio);
}

int SX127x::check_irq()
{
    return liblora_sx127x_check_irq(&m_radio);
}

int SX127x::config_interrupt(void(* callback)(void))
{
    return liblora_sx127x_config_interrupt(&m_radio, callback);
}
