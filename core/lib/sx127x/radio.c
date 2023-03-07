#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./radio.h"
#include "./reg.h"
#include "../board.h"

/**============================================
 *               OPMODE
 *=============================================**/
#define LIBLORA_SX127x_OPMODE_SLEEP             0x00
#define LIBLORA_SX127x_OPMODE_STDBY             0x01
#define LIBLORA_SX127x_OPMODE_FSTX              0x02
#define LIBLORA_SX127x_OPMODE_TX                0x03
#define LIBLORA_SX127x_OPMODE_FSRX              0x04
#define LIBLORA_SX127x_OPMODE_RX_CONT           0x05
#define LIBLORA_SX127x_OPMODE_RX_SINGLE         0x06
#define LIBLORA_SX127x_OPMODE_CAD               0x07

/**============================================
 *               IRQ (MASK)
 *=============================================**/
#define LIBLORA_SX127x_IRQ_CAD_DETECTED         0x01 // -------1
#define LIBLORA_SX127x_IRQ_FHSS_CHANGE_CHAN     0x02 // ------1-
#define LIBLORA_SX127x_IRQ_CAD_DONE             0x04 // -----1--
#define LIBLORA_SX127x_IRQ_TX_DONE              0x08 // ----1---
#define LIBLORA_SX127x_IRQ_VALID_HEADER         0x10 // ---1----
#define LIBLORA_SX127x_IRQ_PAYLOAD_CRC_ERR      0x20 // --1-----
#define LIBLORA_SX127x_IRQ_RX_DONE              0x40 // -1------
#define LIBLORA_SX127x_IRQ_RX_TIMEOUT           0x80 // 1-------

/**============================================
 *               DIO MAPPING
 *=============================================**/
#define LIBLORA_SX127x_DIO0_RX_DONE             0x00 // 00------
#define LIBLORA_SX127x_DIO0_TX_DONE             0x01 // 01------
#define LIBLORA_SX127x_DIO0_CAD_DONE            0x02 // 10------
#define LIBLORA_SX127x_DIO0_NOP                 0xC0 // 11------

#define LIBLORA_SX127x_DIO1_RX_TIMEOUT          0x00 // --00----
#define LIBLORA_SX127x_DIO1_FHSS_CHANGE_CHAN    0x10 // --01----
#define LIBLORA_SX127x_DIO1_CAD_DETECTED        0x20 // --10----
#define LIBLORA_SX127x_DIO1_NOP                 0x30 // --11----

#define LIBLORA_SX127x_DIO2_FHSS_CHANGE_CHAN    0x00 // ----00--
#define LIBLORA_SX127x_DIO2_NOP                 0x0C // ----11--

#define LIBLORA_SX127x_DIO3_CAD_DONE            0x00 // ------00
#define LIBLORA_SX127x_DIO3_VALID_HEADER        0x01 // ------01
#define LIBLORA_SX127x_DIO3_PAYLOAD_CRC_ERR     0x02 // ------10
#define LIBLORA_SX127x_DIO3_NOP                 0x03 // ------11

// Initialisation
int liblora_sx127x_init(liblora_sx127x_radio_t *radio, long freq, uint8_t sf, uint8_t bw)
{
    uint8_t ver;

    liblora_board_gpio_setup(radio->userdata);
    liblora_board_gpio_mode(radio->dio0, LIBLORA_BOARD_GPIO_MODE_INPUT, radio->userdata);
    liblora_board_gpio_mode(radio->rst, LIBLORA_BOARD_GPIO_MODE_OUTPUT, radio->userdata);

    // Reset chip
    if (radio->rst != -1)
    {
        liblora_board_gpio_write(radio->rst, LIBLORA_BOARD_GPIO_STATE_LOW, radio->userdata);
        liblora_board_wait(100, radio->userdata);
        liblora_board_gpio_write(radio->rst, LIBLORA_BOARD_GPIO_STATE_HIGH, radio->userdata);
        liblora_board_wait(100, radio->userdata);
    }

    if (liblora_board_spi_open(radio->dev, radio->userdata) == -1)
    {
        printf("Failed to open spi device\n");
        return -1;
    }

    // check version
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_VERSION, &ver, radio->userdata);
    if (ver != 0x12)
    {
        printf("Wrong device version: %i\n", ver);
        return -1;
    }

    // sleep & lora
    liblora_sx127x_opmode_t curr_opmode = liblora_sx127x_opmode_read(radio);
    curr_opmode.lora = true;
    curr_opmode.mode = LIBLORA_SX127x_OPMODE_SLEEP;
    liblora_sx127x_opmode_write(radio, curr_opmode);

    // Configure
    liblora_sx127x_config_frequency(radio, freq);
    // liblora_sx127x_config_bandwidth(radio, bw, true);
    // liblora_sx127x_config_spreading_factor(radio, sf, true);
    liblora_sx127x_config_pa(radio, true, 17, 4);
    liblora_sx127x_config_lna(radio, true, 0);
    liblora_sx127x_config_agc(radio, true);

    // Reset fifo addresses
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_FIFO_TX_BASE_ADDR, 0x00, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_FIFO_RX_BASE_ADDR, 0x00, radio->userdata);

    // liblora_sx127x_sleep(radio);
    liblora_sx127x_idle(radio);
    return 0;
}

// opmode (private)
liblora_sx127x_opmode_t liblora_sx127x_opmode_read(liblora_sx127x_radio_t *radio)
{
    uint8_t _low_freq, _shared_reg, _lora;

    liblora_sx127x_opmode_t ret;
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_OPMODE_DEV_MODE, &(ret.mode), radio->userdata);
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_OPMODE_LF_MODE, &_low_freq, radio->userdata);
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_OPMODE_SHARED_ACCESS, &_shared_reg, radio->userdata);
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_OPMODE_LR_MODE, &_lora, radio->userdata);

    ret.low_freq = _low_freq == 1 ? true : false;
    ret.shared_reg = _shared_reg == 1 ? true : false;
    ret.lora = _lora == 1 ? true : false;

    return ret;
}

void liblora_sx127x_opmode_write(liblora_sx127x_radio_t *radio, liblora_sx127x_opmode_t _opmode)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_OPMODE_DEV_MODE, _opmode.mode, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_OPMODE_LF_MODE, _opmode.low_freq, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_OPMODE_SHARED_ACCESS, _opmode.shared_reg, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_OPMODE_LR_MODE, _opmode.lora, radio->userdata);
}

// opmode (public)
void liblora_sx127x_sleep(liblora_sx127x_radio_t *radio)
{
    liblora_sx127x_opmode_t curr = liblora_sx127x_opmode_read(radio);
    curr.mode = LIBLORA_SX127x_OPMODE_SLEEP;
    liblora_sx127x_opmode_write(radio, curr);
}

void liblora_sx127x_idle(liblora_sx127x_radio_t *radio)
{
    liblora_sx127x_opmode_t curr = liblora_sx127x_opmode_read(radio);
    curr.mode = LIBLORA_SX127x_OPMODE_STDBY;
    liblora_sx127x_opmode_write(radio, curr);
}

bool liblora_sx127x_recv(liblora_sx127x_radio_t *radio, bool continuous)
{
    // TODO: Check if transmitting, if so return false

    uint8_t _mode = continuous ? LIBLORA_SX127x_OPMODE_RX_CONT : LIBLORA_SX127x_OPMODE_RX_SINGLE;
    if (liblora_sx127x_opmode_read(radio).mode != _mode)
    {
        // DIO0=RXDONE, DIO1=NOP, DIO2=NOP, DIO3=NOP
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_DIO_MAPPING1, LIBLORA_SX127x_DIO0_RX_DONE | LIBLORA_SX127x_DIO1_NOP | LIBLORA_SX127x_DIO2_NOP | LIBLORA_SX127x_DIO3_NOP, radio->userdata);

        // Reset IRQ flags and set IRQ flags mask
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS, 0xFF, radio->userdata);
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS_MASK, LIBLORA_SX127x_IRQ_TX_DONE | LIBLORA_SX127x_IRQ_CAD_DONE | LIBLORA_SX127x_IRQ_FHSS_CHANGE_CHAN | LIBLORA_SX127x_IRQ_CAD_DETECTED, radio->userdata);

        // Change OpMode
        liblora_sx127x_opmode_t curr = liblora_sx127x_opmode_read(radio);
        curr.mode = _mode;
        liblora_sx127x_opmode_write(radio, curr);
    }

    return true;
}

void liblora_sx127x_send(liblora_sx127x_radio_t *radio, bool async)
{
    uint8_t irq;

    // DIO0=TXDONE, DIO1=NOP, DIO2=NOP, DIO3=NOP
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_DIO_MAPPING1, LIBLORA_SX127x_DIO0_TX_DONE | LIBLORA_SX127x_DIO1_NOP | LIBLORA_SX127x_DIO2_NOP | LIBLORA_SX127x_DIO3_NOP, radio->userdata);

    // Reset IRQ flags and set IRQ flags mask
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS, 0xFF, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS_MASK, ~(LIBLORA_SX127x_IRQ_TX_DONE), radio->userdata);

    // OPMODE_TX: Transmit packet
    liblora_sx127x_opmode_t curr = liblora_sx127x_opmode_read(radio);
    curr.mode = LIBLORA_SX127x_OPMODE_TX;
    liblora_sx127x_opmode_write(radio, curr);

    // wait for TxDone
    if (!async)
    {
        liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS, &irq, radio->userdata);
        while ((irq & LIBLORA_SX127x_IRQ_TX_DONE) == 0)
            liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS, &irq, radio->userdata);
    }

    // FIXME: Remove code below
    // reset DIO mapping & IRQ mask
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS_MASK, 0xFF, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_DIO_MAPPING1, 0xFF, radio->userdata);
}

// config (public)
void liblora_sx127x_config_frequency(liblora_sx127x_radio_t *radio, long freq)
{
    uint64_t frf = ((uint64_t)freq << 19) / 32000000;
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_FRF_MSB, (uint8_t)(frf >> 16), radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_FRF_MID, (uint8_t)(frf >> 8), radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_FRF_LSB, (uint8_t)(frf >> 0), radio->userdata);
}

void liblora_sx127x_config_bandwidth(liblora_sx127x_radio_t *radio, uint8_t bw, bool optimize)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_MOD_CFG1_BW, bw, radio->userdata);

    if (optimize && bw == LIBLORA_SX127x_BW_007_80)
        liblora_sx127x_config_high_bw_optimization(radio, true);
    else
        liblora_sx127x_config_high_bw_optimization(radio, false);
}

void liblora_sx127x_config_spreading_factor(liblora_sx127x_radio_t *radio, uint8_t sf, bool optimize)
{
    if (sf == LIBLORA_SX127x_SF_6)
    {
        liblora_sx127x_config_detection_optimization(radio, 0x05);
        liblora_sx127x_config_detection_threshold(radio, 0x0C);
    }
    else
    {
        liblora_sx127x_config_detection_optimization(radio, 0x03);
        liblora_sx127x_config_detection_optimization(radio, 0xA);
    }

    if (sf == LIBLORA_SX127x_SF_11 || sf == LIBLORA_SX127x_SF_12)
        liblora_sx127x_config_low_data_rate_optimization(radio, true);
    else
        liblora_sx127x_config_low_data_rate_optimization(radio, false);

    // TODO: understand why ?
    // if (sf == sf_t::SF10 || sf == sf_t::SF11 || sf == sf_t::SF12) {
    //     liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_SYMB_TIMEOUT_LSB, 0x05);
    // } else {
    //     liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_SYMB_TIMEOUT_LSB, 0x08);
    // }
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_MOD_CFG2_SF, sf, radio->userdata);
}

void liblora_sx127x_config_coding_rate(liblora_sx127x_radio_t *radio, uint8_t cr)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_MOD_CFG1_CR, cr, radio->userdata);
}

void liblora_sx127x_config_invert_iq(liblora_sx127x_radio_t *radio, bool enable, bool rx)
{
    // FIXME: Understand which flag must be set RX or TX
    if (enable)
    {
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_INVERT_IQ_RX, 0x01, radio->userdata);
        // liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_INVERT_IQ_TX, 0x01);
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_INVERT_IQ2, 0x19, radio->userdata);
    }
    else
    {
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_INVERT_IQ_RX, 0x00, radio->userdata);
        // liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_INVERT_IQ_TX, 0x00);
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_INVERT_IQ2, 0x1D, radio->userdata);
    }
}

void liblora_sx127x_config_sync_word(liblora_sx127x_radio_t *radio, uint8_t value)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_SYNC_WORD, value, radio->userdata);
}

// config/optimization (private)
void liblora_sx127x_config_low_data_rate_optimization(liblora_sx127x_radio_t *radio, bool enable)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_CFG3_LORA_LOW_DR_OPTI, enable ? 1 : 0, radio->userdata);
}

void liblora_sx127x_config_detection_optimization(liblora_sx127x_radio_t *radio, uint8_t value)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_DETECT_OPTI, value, radio->userdata);
}

void liblora_sx127x_config_detection_threshold(liblora_sx127x_radio_t *radio, uint8_t value)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_DETECT_THRESH, value, radio->userdata);
}

void liblora_sx127x_config_high_bw_optimization(liblora_sx127x_radio_t *radio, bool enable)
{
    // FIXME: Values differs for frequencies
    if (enable)
    {
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_HIGH_BW_OPTI1, 0x02, radio->userdata);
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_HIGH_BW_OPTI2, 0x64, radio->userdata);
    }
    else
    {
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_HIGH_BW_OPTI1, 0x03, radio->userdata);

        // Automatically reset by the chip
        // liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_HIGH_BW_OPTI2, 0x65);
    }
}

// config (private)
void liblora_sx127x_config_crc(liblora_sx127x_radio_t *radio, bool enable)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_MOD_CFG2_RX_PAYLOAD_CRC, enable ? 1 : 0, radio->userdata);
}

void liblora_sx127x_config_header_mode(liblora_sx127x_radio_t *radio, bool _explicit)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_MOD_CFG1_HEADER_MODE, _explicit ? 0 : 1, radio->userdata);
}

void liblora_sx127x_config_symb_timeout(liblora_sx127x_radio_t *radio, uint16_t timeout)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_MOD_CFG2_SYMB_TMOUT_MSB, (timeout >> 8) & 0x3, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_MOD_SYMB_TMOUT_LSB, timeout & 0xFF, radio->userdata);
}

void liblora_sx127x_config_preamble_len(liblora_sx127x_radio_t *radio, uint16_t len)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_PREAMBLE_LEN_MSB, len >> 8, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_PREAMBLE_LEN_LSB, len & 0xFF, radio->userdata);
}

void liblora_sx127x_config_max_payload_len(liblora_sx127x_radio_t *radio, uint8_t len)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_MAX_PAYLOAD_LEN, len, radio->userdata);
}

void liblora_sx127x_config_hop_period(liblora_sx127x_radio_t *radio, uint8_t value)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_FREQ_HOP_PERDIOD, value, radio->userdata);
}

void liblora_sx127x_config_pa(liblora_sx127x_radio_t *radio, bool boost, uint8_t power, uint8_t max_power)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_PA_OUTPUT_POW, power - 2, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_PA_MAX_POW, max_power, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_PA_SELECT, boost ? 1 : 0, radio->userdata);
}

void liblora_sx127x_config_agc(liblora_sx127x_radio_t *radio, bool enable)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_CFG3_AGC_ON, enable ? 1 : 0, radio->userdata);
}

void liblora_sx127x_config_pa_ramp(liblora_sx127x_radio_t *radio, uint8_t ramp)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_PA_RAMP, ramp, radio->userdata);
}

void liblora_sx127x_config_ocp(liblora_sx127x_radio_t *radio, bool enable, uint8_t trim)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_OCP_ON, enable ? 1 : 0, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_OCP_TRIM, trim, radio->userdata);
}

void liblora_sx127x_config_lna(liblora_sx127x_radio_t *radio, bool boost, uint8_t gain)
{
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LNA_BOOST_HF, boost ? 0 : 3, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LNA_GAIN, gain, radio->userdata);
}

// fifo (private)
void liblora_sx127x_fifo_write(liblora_sx127x_radio_t *radio, uint8_t *buffer, uint8_t len)
{
    liblora_sx127x_reg_writeb(radio->dev, LIBLORA_SX127x_REG_FIFO, buffer, len, radio->userdata);
}

void liblora_sx127x_fifo_read(liblora_sx127x_radio_t *radio, uint8_t *buffer, uint8_t len)
{
    // FIXME: try using liblora_driver_spi_read_burst
    for (int i = 0; i < len; i++)
        liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_FIFO, &buffer[i], radio->userdata);
}

// packet (private)
uint8_t liblora_sx127x_packet_size(liblora_sx127x_radio_t *radio)
{
    uint8_t pkt_size;
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_FIFO_RX_BYTES_NB, &pkt_size, radio->userdata);
    return pkt_size;
}

uint8_t liblora_sx127x_packet_rssi(liblora_sx127x_radio_t *radio)
{
    // FIXME: Fix correction based on freq
    uint8_t rssi;
    int rssi_corr = 157;
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_PKT_RSSI, &rssi, radio->userdata);
    return rssi - rssi_corr;
}

int8_t liblora_sx127x_packet_snr(liblora_sx127x_radio_t *radio)
{
    uint8_t snr;
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_PKT_SNR, &snr, radio->userdata);
    return ((int8_t)snr) >> 2;
}

uint8_t liblora_sx127x_packet_strength(liblora_sx127x_radio_t *radio)
{
    int8_t snr = liblora_sx127x_packet_snr(radio);
    if (snr < 0)
        return liblora_sx127x_packet_rssi(radio) + snr;

    return liblora_sx127x_packet_rssi(radio);
}

// packet (public)
liblora_sx127x_packet_t liblora_sx127x_packet_read(liblora_sx127x_radio_t *radio)
{
    // FIXME: In Explicit Header Mode (default), use Crc from REG_HOP_CHANNEL
    uint8_t irq_flags, curr_addr, base_addr;

    liblora_sx127x_packet_t pkt = {
        .buffer = {},
        .size = 0,
        .pkt_snr = 0,
        .pkt_rssi = 0,
        .rssi = 0,
        .strength = 0};
    
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS, &irq_flags, radio->userdata);
    if ((irq_flags & LIBLORA_SX127x_IRQ_RX_DONE) != 0)
    {
        // clear RX_DONE IRQ flags
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS, LIBLORA_SX127x_IRQ_RX_DONE | LIBLORA_SX127x_IRQ_VALID_HEADER, radio->userdata);

        // check IRQ flags
        if ((irq_flags & (LIBLORA_SX127x_IRQ_RX_TIMEOUT | LIBLORA_SX127x_IRQ_PAYLOAD_CRC_ERR)) != 0)
        {
            // TODO: Something went wrong
        }
        else
        {
            pkt.size = liblora_sx127x_packet_size(radio);

            liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_FIFO_CURR_ADDR, &curr_addr, radio->userdata);
            liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_FIFO_ADDR_PTR, curr_addr, radio->userdata);
            liblora_sx127x_fifo_read(radio, pkt.buffer, pkt.size);

            pkt.pkt_snr = liblora_sx127x_packet_snr(radio);
            pkt.pkt_rssi = liblora_sx127x_packet_rssi(radio);
            pkt.rssi = liblora_sx127x_rssi(radio);
            pkt.strength = liblora_sx127x_packet_strength(radio);
        }

        // reset FIFO addr to RX base addr & reset IRQ flags
        liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_FIFO_RX_BASE_ADDR, &base_addr, radio->userdata);
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_FIFO_ADDR_PTR, base_addr, radio->userdata);
        liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS, irq_flags, radio->userdata);
    }

    return pkt;
}

void liblora_sx127x_packet_write(liblora_sx127x_radio_t *radio, uint8_t *buffer, uint8_t len)
{
    // TODO: Logic in Explicit Header Mode (default)
    uint8_t base_addr;

    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS, 0xFF, radio->userdata);

    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_FIFO_TX_BASE_ADDR, &base_addr, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_FIFO_ADDR_PTR, base_addr, radio->userdata);
    liblora_sx127x_reg_write(radio->dev, LIBLORA_SX127x_REG_LORA_PAYLOAD_LEN, len, radio->userdata);
    liblora_sx127x_fifo_write(radio, buffer, len);
}

// other (public)
void liblora_sx127x_afc_update(liblora_sx127x_radio_t *radio)
{
    // TODO:
    // 1. read the frequency error
    // 2. Convert from Hertz to PPM
    // 3. Calculate the new RF centre frequency & update register
    // 4. Calculate offset, convert to 8bit 2's complement 
    //    & update register 0x27
}

uint16_t liblora_sx127x_valid_header_count(liblora_sx127x_radio_t *radio)
{
    uint8_t count_msb, count_lsb;

    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_RX_HEADER_CNT_MSB, &count_msb, radio->userdata);
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_RX_HEADER_CNT_LSB, &count_lsb, radio->userdata);

    return ((uint16_t)(count_msb) << 8) | count_lsb;
}

uint16_t liblora_sx127x_valid_packet_count(liblora_sx127x_radio_t *radio)
{
    uint8_t count_msb, count_lsb;

    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_RX_PKT_CNT_MSB, &count_msb, radio->userdata);
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_RX_PKT_CNT_LSB, &count_lsb, radio->userdata);

    return ((uint16_t)(count_msb) << 8) | count_lsb;
}

int64_t liblora_sx127x_frequency_error(liblora_sx127x_radio_t *radio)
{
    // TODO: Fix this code
    // uint8_t fei_msb = liblora_driver_spi_read(LIBLORA_SX127x_REG_FEI_MSB, radio->driver_config) & 0xF;

    // int32_t fei_val = (static_cast<int32_t>(fei_msb) & 0x7) << 16;
    // fei_val |= (static_cast<int32_t>(liblora_driver_spi_read(LIBLORA_SX127x_REG_FEI_MID, radio->driver_config)) << 8);
    // fei_val |= static_cast<int32_t>(liblora_driver_spi_read(LIBLORA_SX127x_REG_FEI_LSB, radio->driver_config));

    // Check sign bit
    // if (fei_msb & 0x8)
    //     fei_val -= 524288;

    // float ferr = ((static_cast<float>(fei_val) * (1L << 24)) / 32e6) * (this->get_bandwidth() / 500000.0f);
    // return static_cast<int64_t>(ferr);
    return 0;
}

uint8_t liblora_sx127x_rssi(liblora_sx127x_radio_t *radio)
{
    // FIXME: Fix correction based on freq
    int rssi_corr = 157;

    uint8_t rssi;
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_RSSI, &rssi, radio->userdata);

    return rssi - rssi_corr;
}

uint8_t liblora_sx127x_random(liblora_sx127x_radio_t *radio)
{
    uint8_t random;
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_RSSI_WIDEBAND, &random, radio->userdata);
    return random;
}

liblora_sx127x_modem_status_t liblora_sx127x_modem_status(liblora_sx127x_radio_t *radio)
{
    uint8_t curr;
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_MODEM_STATUS, &curr, radio->userdata);

    liblora_sx127x_modem_status_t options[] = {
        SIGNAL_DETECTED,
        SIGNAL_SYNCED,
        RX_ON_GOING,
        HEADER_INFO_VALID,
        MODEM_CLEAR};

    for (int i = 0; i < 5; i++)
        if (curr & options[i])
            return options[i];

    return UNKNOWN;
}

// interrupts
int liblora_sx127x_check_irq(liblora_sx127x_radio_t *radio)
{
    uint8_t irq_flags;
    liblora_sx127x_reg_read(radio->dev, LIBLORA_SX127x_REG_LORA_IRQ_FLAGS, &irq_flags, radio->userdata);

    if ((irq_flags & LIBLORA_SX127x_IRQ_RX_DONE) > 0) // RX_DONE
    {
        liblora_sx127x_packet_t pkt = liblora_sx127x_packet_read(radio);
        if (radio->onrx != NULL)
            radio->onrx(pkt);
    }
    else if ((irq_flags & LIBLORA_SX127x_IRQ_TX_DONE) > 0) // TX_DONE
    {
        if (radio->ontx != NULL)
            radio->ontx();
    }

    return 0;
}

int liblora_sx127x_config_interrupt(liblora_sx127x_radio_t *radio, void (*callback)(void))
{
    return liblora_board_gpio_attach_interrupt(
        radio->dio0,
        LIBLORA_BOARD_GPIO_EDGE_RISING,
        callback,
        radio->userdata);
}
