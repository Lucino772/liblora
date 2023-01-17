#include "../include/liblora/rf95.h"


RF95::RF95(int dio0, int rst, int spi_dev, int spi_ss, int spi_clck_speed)
{
    _dev.spi_dev = spi_dev;
    _dev.spi_ss = spi_ss;
    _dev.spi_clck_speed = spi_clck_speed;
    _dev.usb = false;

    _radio.dio0 = dio0;
    _radio.rst = rst;
    _radio.com = &_dev;
    _radio.onrx = nullptr;
    _radio.ontx = nullptr;
}

RF95::~RF95() {
    _radio.com = nullptr;
    _radio.onrx = nullptr;
    _radio.ontx = nullptr;
}

int RF95::init(long freq, uint8_t sf, uint8_t bw)
{
    return liblora_rf95_init(&_radio, freq, sf, bw);
}

void RF95::onRx(void (*callback)(liblora_rf95_packet_t))
{
    _radio.onrx = callback;
}

void RF95::onTx(void (*callback)(void))
{
    _radio.ontx = callback;
}


void RF95::sleep()
{
    liblora_rf95_sleep(&_radio);
}

void RF95::idle()
{
    liblora_rf95_idle(&_radio);
}

bool RF95::recv(bool continuous)
{
    return liblora_rf95_recv(&_radio, continuous);
}

void RF95::send(bool async)
{
    liblora_rf95_send(&_radio, async);
}


void RF95::setFrequency(long freq)
{
    liblora_rf95_config_frequency(&_radio, freq);
}

void RF95::setBandwidth(uint8_t bw)
{
    liblora_rf95_config_bandwidth(&_radio, bw, true);
}

void RF95::setSpreadingFactor(uint8_t sf)
{
    liblora_rf95_config_spreading_factor(&_radio, sf, true);
}

void RF95::setCodingRate(uint8_t cr)
{
    liblora_rf95_config_coding_rate(&_radio, cr);
}

void RF95::setInvertIQ(bool enable, bool rx)
{
    liblora_rf95_config_invert_iq(&_radio, enable, rx);
}

void RF95::setSyncWord(uint8_t value)
{
    liblora_rf95_config_sync_word(&_radio, value);
}


liblora_rf95_packet_t RF95::readPacket()
{
    return liblora_rf95_packet_read(&_radio);
}

void RF95::writePacket(uint8_t* buffer, int len)
{
    liblora_rf95_packet_write(&_radio, buffer, len);
}


uint16_t RF95::getValidHeaderCount()
{
    return liblora_rf95_valid_header_count(&_radio);
}

uint16_t RF95::getValidPacketCount()
{
    return liblora_rf95_valid_packet_count(&_radio);
}

int64_t RF95::getFrequencyError()
{
    return liblora_rf95_frequency_error(&_radio);
}

uint8_t RF95::getRssi()
{
    return liblora_rf95_rssi(&_radio);
}

uint8_t RF95::getRandom()
{
    return liblora_rf95_random(&_radio);
}

liblora_rf95_modem_status_t RF95::getModemStatus()
{
    return liblora_rf95_modem_status(&_radio);
}


int RF95::checkIRQ()
{
    return liblora_rf95_check_irq(&_radio);
}

int RF95::setDio0Interrupt(void (*callback)(void))
{
    return liblora_rf95_config_interrupt(&_radio, callback);
}
