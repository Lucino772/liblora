#include "Arduino.h"
#include "liblora/rf95.h"

RF95 radio(2, 9, 0, 10, (int)500000);

void on_receive(liblora_rf95_packet_t pkt)
{
    if (pkt.size > 0)
    {
        Serial.print("Packet received (");
        Serial.print(pkt.size);
        Serial.print(" bytes): ");
        for (int i = 0; i < pkt.size; i++)
        {
            Serial.print(pkt.buffer[i]);
        }
        Serial.println();
    } else {
        Serial.println("Something went wrong !");
    }
}

void radio1_interrupt_dio0()
{
    radio.checkIRQ();
}

void setup()
{
    Serial.begin(9600);

    radio.init(8681E5, LIBLORA_RF95_SF_7, LIBLORA_RF95_BW_125);
    radio.onRx(&on_receive);
    radio.recv(true);
}

void loop()
{
}