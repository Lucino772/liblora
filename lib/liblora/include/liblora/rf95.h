#include <cstdint>
#include <vector>

#include "c/rf95.h"
#include "c/hal.h"

class RF95
{
public:
    RF95(int dio0, int rst, int spi_dev, int spi_ss, int spi_clck_speed);
    ~RF95();

    int init(long freq, uint8_t sf, uint8_t bw);
    void onRx(void (*callback)(liblora_rf95_packet_t));
    void onTx(void (*callback)(void));

    void sleep();
    void idle();
    bool recv(bool continuous = false);
    void send(bool async = false);

    void setFrequency(long freq);
    void setBandwidth(uint8_t bw);
    void setSpreadingFactor(uint8_t sf);
    void setCodingRate(uint8_t cr);
    void setInvertIQ(bool enable);
    void setSyncWord(uint8_t value);

    liblora_rf95_packet_t readPacket();
    void writePacket(uint8_t *buffer, int len);

    uint16_t getValidHeaderCount();
    uint16_t getValidPacketCount();
    int64_t getFrequencyError();
    uint8_t getRssi();
    uint8_t getRandom();
    liblora_rf95_modem_status_t getModemStatus();

    int checkIRQ();
    int setDio0Interrupt(void (*callback)(void));

private:
    liblora_rf95_radio_t _radio;
    liblora_com_dev_t _dev;
};