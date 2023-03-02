#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

#include <liblora/board.h>
#include <liblora/sx127x/radio.h>

liblora_com_dev_t com = { 0, 6, (int)500000, false };
liblora_sx127x_radio_t radio = LIBLORA_SX127x_RADIO_INIT(&com);
bool running = true;

void sig_term_handler(int sig)
{
    liblora_sx127x_sleep(&radio);
    printf("Terminated\n");
    running = false;
}

void on_receive(liblora_sx127x_packet_t pkt)
{
    if (pkt.size > 0)
    {
        printf("Packet received (%i bytes): %s\n", pkt.size, pkt.buffer);
    } else {
        printf("Something went wrong !\n");
    }
}

void radio1_interrupt_dio0()
{
    liblora_sx127x_check_irq(&radio);
}

int main()
{
    signal(SIGINT, sig_term_handler);

    if (liblora_sx127x_init(&radio, 8681E5, LIBLORA_SX127x_SF_7, LIBLORA_SX127x_BW_125_00) != 0)
    {
        perror("Failed to initialize radio\n");
        return 1;
    }

    // Config RX callback and attach interrupt
    radio.onrx = &on_receive;
    liblora_sx127x_config_interrupt(&radio, &radio1_interrupt_dio0);

    if (!liblora_sx127x_recv(&radio, true))
        perror("Failed to put in RX_CONT mode");
    else
        while (running);

    liblora_sx127x_sleep(&radio);
    return 0;
}