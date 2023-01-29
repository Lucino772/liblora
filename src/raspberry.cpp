#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

#include <liblora/rf95.h>

RF95 radio(7, 0, 0, 6, (int)500000);
bool running = true;

void sig_term_handler(int sig)
{
    radio.sleep();
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

void radio1_interrupt_dio0()
{
    radio.checkIRQ();
}

int main()
{
    signal(SIGINT, sig_term_handler);

    if (radio.init(8681E5, LIBLORA_RF95_SF_7, LIBLORA_RF95_BW_125) != 0)
    {
        perror("Failed to initialize radio\n");
        return 1;
    }

    // Config RX callback and attach interrupt
    radio.onRx(&on_receive);
    radio.setDio0Interrupt(&radio1_interrupt_dio0);

    if (!radio.recv(true))
        perror("Failed to put in RX_CONT mode");
    else
        while (running);

    radio.sleep();
    return 0;
}