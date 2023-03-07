#include <cstdio>
#include <csignal>

#include <sx127x.hpp>

SX127x radio(7, 0, 0, 6, 500000);
bool running = true;

void sig_term_handler(int sig)
{
    radio.sleep();
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
    radio.check_irq();
}

int main()
{
    signal(SIGINT, sig_term_handler);

    if (radio.init(8681E5, LIBLORA_SX127x_SF_7, LIBLORA_SX127x_BW_125_00) != 0)
    {
        perror("Failed to initialize radio\n");
        return 1;
    }

    // Config RX callback and attach interrupt
    radio.onrx(&on_receive);
    radio.config_interrupt(&radio1_interrupt_dio0);

    if (!radio.recv(true))
    {
        perror("Failed to put in RX_CONT mode\n");
    }
    else
    {
        printf("Ready !\n");
        while (running == true) {
            __asm__("nop");
        }
        printf("Done %i !\n", running);
    }

    radio.sleep();
    return 0;
}