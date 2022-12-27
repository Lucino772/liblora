#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

#include <liblora/drivers.h>
#include <liblora/rf95.h>
#include <wiringPi.h>

liblora_wiringpi_config_t driver_config = LIBLORA_DRIVER_WIRINGPI_INIT;
liblora_rf95_radio_t radio = LIBLORA_RF95_RADIO_INIT;
bool running = true;

void sig_term_handler(int sig)
{
    liblora_rf95_sleep(&radio);
    printf("Terminated\n");
    running = false;
}

void on_receive(liblora_rf95_packet_t pkt)
{
    if (pkt.size > 0)
    {
        printf("Packet received (%i bytes): %s\n", pkt.size, pkt.buffer);
    } else {
        printf("Something went wrong !\n");
    }
}


int main()
{
    signal(SIGINT, sig_term_handler);
    radio.driver_config = driver_config;

    if (liblora_rf95_init(&radio, 8681E5, LIBLORA_RF95_SF_7, LIBLORA_RF95_BW_125) != 0)
    {
        perror("Failed to initialize radio\n");
        return 1;
    }

    // configure interrupts
    // wiringPiISR(7, INT_EDGE_RISING, on_dio0_rise);
    liblora_rf95_onrx(&radio, on_receive);

    if (!liblora_rf95_recv(&radio, true))
        perror("Failed to put in RX_CONT mode");
    else
        while (running);

    liblora_rf95_sleep(&radio);
    return 0;
}