/*
 * Created on Fri Aug 12 2022
 *
 * Copyright (c) 2022 Lucino772
 */
#include <stdlib.h>

#include "_rf95.h"

// Initialisation
liblora_rf95_radio_t* liblora_rf95_radio(liblora_driver_t* driver, int dio0, int rst)
{
    liblora_driver_setup(driver);

    liblora_rf95_radio_t* radio = (liblora_rf95_radio_t*)malloc(sizeof(liblora_rf95_radio_t));
    radio->driver = driver;
    radio->dio0 = dio0;
    radio->rst = rst;
    radio->onrx = NULL;
    radio->ontx = NULL;

    return radio;
}

int liblora_rf95_init(liblora_rf95_radio_t *radio, long freq, uint8_t sf, uint8_t bw)
{
    liblora_driver_pin_mode(radio->driver, radio->dio0, INPUT);
    liblora_driver_pin_mode(radio->driver, radio->rst, OUTPUT);

    // Reset chip
    if (radio->rst != -1)
    {
        liblora_driver_pin_write(radio->driver, radio->rst, LOW);
        liblora_driver_delay(radio->driver, 100);
        liblora_driver_pin_write(radio->driver, radio->rst, HIGH);
        liblora_driver_delay(radio->driver, 100);
    }

    if (liblora_driver_spi_init(radio->driver) == -1)
        return -1;

    // check version
    uint8_t ver = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_VERSION);
    if (ver != 0x12)
        return -1;

    // sleep & lora
    liblora_rf95_opmode_t curr_opmode = liblora_rf95_opmode_read(radio);
    curr_opmode.lora = true;
    curr_opmode.mode = LIBLORA_RF95_OPMODE_SLEEP;
    liblora_rf95_opmode_write(radio, curr_opmode);

    // Configure
    liblora_rf95_config_frequency(radio, freq);
    // liblora_rf95_config_bandwidth(radio, bw, true);
    // liblora_rf95_config_spreading_factor(radio, sf, true);
    liblora_rf95_config_pa(radio, true, 17, 4);
    liblora_rf95_config_lna(radio, true, 0);
    liblora_rf95_config_agc(radio, true);

    // Reset fifo addresses
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_FIFO_TX_BASE_ADDR, 0x00);
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_FIFO_RX_BASE_ADDR, 0x00);

    // liblora_rf95_sleep(radio);
    liblora_rf95_idle(radio);
    return 0;
}

void liblora_rf95_delete(liblora_rf95_radio_t* radio)
{
    if (radio == NULL)
        return;

    if (radio->driver != NULL)
        liblora_driver_delete(radio->driver);

    if (radio != NULL)
        free(radio);
}

// opmode (private)
liblora_rf95_opmode_t liblora_rf95_opmode_read(liblora_rf95_radio_t *radio)
{
    uint8_t _opmode = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_OPMODE);
    liblora_rf95_opmode_t ret = {
        .lora = (_opmode & 0x80) == 0x80,       // lora mode
        .shared_reg = (_opmode & 0x40) == 0x40, // shared reg
        .low_freq = (_opmode & 0x04) == 0x04,   // low freq. on
        .mode = (uint8_t)(_opmode | 0x07)};

    return ret;
}

void liblora_rf95_opmode_write(liblora_rf95_radio_t *radio, liblora_rf95_opmode_t _opmode)
{
    uint8_t _new = _opmode.mode;
    if (_opmode.lora)
        _new |= 0x80;
    if (_opmode.shared_reg)
        _new |= 0x40;
    if (_opmode.low_freq)
        _new |= 0x04;

    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_OPMODE, _new);
}

// opmode (public)
void liblora_rf95_sleep(liblora_rf95_radio_t *radio)
{
    liblora_rf95_opmode_t curr = liblora_rf95_opmode_read(radio);
    curr.mode = LIBLORA_RF95_OPMODE_SLEEP;
    liblora_rf95_opmode_write(radio, curr);
}

void liblora_rf95_idle(liblora_rf95_radio_t *radio)
{
    liblora_rf95_opmode_t curr = liblora_rf95_opmode_read(radio);
    curr.mode = LIBLORA_RF95_OPMODE_STDBY;
    liblora_rf95_opmode_write(radio, curr);
}

bool liblora_rf95_recv(liblora_rf95_radio_t *radio, bool continuous)
{
    // TODO: Check if transmitting, if so return false

    uint8_t _mode = continuous ? LIBLORA_RF95_OPMODE_RX_CONT : LIBLORA_RF95_OPMODE_RX_SINGLE;
    if (liblora_rf95_opmode_read(radio).mode != _mode)
    {
        // DIO0=RXDONE, DIO1=NOP, DIO2=NOP, DIO3=NOP
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_DIO_MAPPING_1, LIBLORA_RF95_DIO0_RX_DONE | LIBLORA_RF95_DIO1_NOP | LIBLORA_RF95_DIO2_NOP | LIBLORA_RF95_DIO3_NOP);

        // Reset IRQ flags and set IRQ flags mask
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS, 0xFF);
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS_MASK, LIBLORA_RF95_IRQ_TX_DONE | LIBLORA_RF95_IRQ_CAD_DONE | LIBLORA_RF95_IRQ_FHSS_CHANGE_CHAN | LIBLORA_RF95_IRQ_CAD_DETECTED);

        // Change OpMode
        liblora_rf95_opmode_t curr = liblora_rf95_opmode_read(radio);
        curr.mode = _mode;
        liblora_rf95_opmode_write(radio, curr);
    }

    return true;
}

void liblora_rf95_send(liblora_rf95_radio_t *radio, bool async)
{
    // DIO0=TXDONE, DIO1=NOP, DIO2=NOP, DIO3=NOP
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_DIO_MAPPING_1, LIBLORA_RF95_DIO0_TX_DONE | LIBLORA_RF95_DIO1_NOP | LIBLORA_RF95_DIO2_NOP | LIBLORA_RF95_DIO3_NOP);

    // Reset IRQ flags and set IRQ flags mask
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS, 0xFF);
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS_MASK, ~(LIBLORA_RF95_IRQ_TX_DONE));

    // OPMODE_TX: Transmit packet
    liblora_rf95_opmode_t curr = liblora_rf95_opmode_read(radio);
    curr.mode = LIBLORA_RF95_OPMODE_TX;
    liblora_rf95_opmode_write(radio, curr);

    // wait for TxDone
    if (!async)
        while ((liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS) & LIBLORA_RF95_IRQ_TX_DONE) == 0);

    // FIXME: Remove code below
    // reset DIO mapping & IRQ mask
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS_MASK, 0xFF);
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_DIO_MAPPING_1, 0xFF);
}

// config (public)
void liblora_rf95_config_frequency(liblora_rf95_radio_t *radio, long freq)
{
    uint64_t frf = ((uint64_t)freq << 19) / 32000000;
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_FR_MSB, (uint8_t)(frf >> 16));
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_FR_MID, (uint8_t)(frf >> 8));
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_FR_LSB, (uint8_t)(frf >> 0));
}

void liblora_rf95_config_bandwidth(liblora_rf95_radio_t *radio, uint8_t bw, bool optimize)
{
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG1);
    uint8_t _new = (bw << 4) | (curr & 0xF);
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG1, _new);

    if (optimize && bw == LIBLORA_RF95_BW_7_8)
        liblora_rf95_config_high_bw_optimization(radio, true);
    else
        liblora_rf95_config_high_bw_optimization(radio, false);
}

void liblora_rf95_config_spreading_factor(liblora_rf95_radio_t *radio, uint8_t sf, bool optimize)
{
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG2);
    uint8_t _new = (sf << 4) | (curr & 0xF);

    if (sf == LIBLORA_RF95_SF_6)
    {
        liblora_rf95_config_detection_optimization(radio, 0x05);
        liblora_rf95_config_detection_threshold(radio, 0x0C);
    }
    else
    {
        liblora_rf95_config_detection_optimization(radio, 0x03);
        liblora_rf95_config_detection_optimization(radio, 0xA);
    }

    if (sf == LIBLORA_RF95_SF_11 || sf == LIBLORA_RF95_SF_12)
        liblora_rf95_config_low_data_rate_optimization(radio, true);
    else
        liblora_rf95_config_low_data_rate_optimization(radio, false);

    // TODO: understand why ?
    // if (sf == sf_t::SF10 || sf == sf_t::SF11 || sf == sf_t::SF12) {
    //     liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_SYMB_TIMEOUT_LSB, 0x05);
    // } else {
    //     liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_SYMB_TIMEOUT_LSB, 0x08);
    // }
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG2, _new);
}

void liblora_rf95_config_coding_rate(liblora_rf95_radio_t *radio, uint8_t cr)
{
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG1);
    uint8_t _new = (curr & 0xF1) | (curr << 1);
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG1, _new);
}

void liblora_rf95_config_invert_iq(liblora_rf95_radio_t *radio, bool enable, bool rx)
{
    // TODO: Check if it's correct
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_INVERT_IQ);
    if (enable)
    {
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_INVERT_IQ, (curr & ~0x41) | 0x40);
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_INVERT_IQ2, 0x19);
    }
    else
    {
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_INVERT_IQ, (curr & ~0x41) | 0x01);
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_INVERT_IQ2, 0x1D);
    }
}

void liblora_rf95_config_sync_word(liblora_rf95_radio_t *radio, uint8_t value)
{
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_SYNC_WORD, value);
}

// config/optimization (private)
void liblora_rf95_config_low_data_rate_optimization(liblora_rf95_radio_t *radio, bool enable)
{
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG3);
    uint8_t _new = curr & ~0x8;
    if (enable)
        _new |= 0x8;

    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG3, _new);
}

void liblora_rf95_config_detection_optimization(liblora_rf95_radio_t *radio, uint8_t value)
{
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_DETECT_OPTI, value);
}

void liblora_rf95_config_detection_threshold(liblora_rf95_radio_t *radio, uint8_t value)
{
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_DETECT_THRESHOLD, value);
}

void liblora_rf95_config_high_bw_optimization(liblora_rf95_radio_t *radio, bool enable)
{
    if (enable)
    {
        // FIX: Values differs for frequencies
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_HIGH_BW_OPTI1, 0x02);
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_HIGH_BW_OPTI2, 0x64);
    }
    else
    {
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_HIGH_BW_OPTI1, 0x03);

        // Automatically reset by the chip
        // liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_HIGH_BW_OPTI2, 0x65);
    }
}

// config (private)
void liblora_rf95_config_crc(liblora_rf95_radio_t *radio, bool enable)
{
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG2);
    uint8_t _new = curr & ~0x04;
    if (enable)
        _new |= 0x04;

    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG2, _new);
}

void liblora_rf95_config_header_mode(liblora_rf95_radio_t *radio, bool _explicit)
{
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG1);
    uint8_t _new = curr & ~0x01;
    if (!_explicit)
        _new |= 0x01;

    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG1, _new);
}

void liblora_rf95_config_symb_timeout(liblora_rf95_radio_t *radio, uint16_t timeout)
{
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG2);
    uint8_t _new = (curr & ~0x3) | ((timeout >> 16) & 0x3);
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG2, _new);
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_SYMB_TIMEOUT_LSB, timeout & 0xFF);
}

void liblora_rf95_config_preamble_len(liblora_rf95_radio_t *radio, uint16_t len)
{
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_PREAMBLE_MSB, len >> 8);
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_PREAMBLE_LSB, len & 0xFF);
}

void liblora_rf95_config_max_payload_len(liblora_rf95_radio_t *radio, uint8_t len)
{
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_MAX_PAYLOAD_LENGTH, len);
}

void liblora_rf95_config_hop_period(liblora_rf95_radio_t *radio, uint8_t value)
{
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_HOP_PERIOD, value);
}

void liblora_rf95_config_pa(liblora_rf95_radio_t *radio, bool boost, uint8_t power, uint8_t max_power)
{
    uint8_t _new = 0;
    if (boost)
        _new |= 0x80;
    _new |= (max_power & 0x7) << 4;
    _new |= (power - 2) & 0xF;

    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_PA_CONFIG, _new);
}

void liblora_rf95_config_agc(liblora_rf95_radio_t *radio, bool enable)
{
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG3);
    uint8_t _new = (curr & ~0x4);
    if (enable)
        _new |= 0x4;

    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_MODEM_CONFIG3, _new);
}

void liblora_rf95_config_pa_ramp(liblora_rf95_radio_t *radio, uint8_t ramp)
{
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_PA_RAMP);
    uint8_t _new = (curr & 0xF) | (ramp & 0xF);
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_PA_RAMP, _new);
}

void liblora_rf95_config_ocp(liblora_rf95_radio_t *radio, bool enable, uint8_t trim)
{
    uint8_t _new = 0;
    if (enable)
        _new |= 0x10;
    _new |= trim & 0xF;

    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_OCP, _new);
}

void liblora_rf95_config_lna(liblora_rf95_radio_t *radio, bool boost, uint8_t gain)
{
    uint8_t _new = 0;
    if (boost)
        _new |= 0x03;
    _new |= gain << 5;

    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_LNA, _new);
}

// fifo (private)
void liblora_rf95_fifo_write(liblora_rf95_radio_t *radio, uint8_t *buffer, uint8_t len)
{
    liblora_driver_spi_write_burst(radio->driver, LIBLORA_RF95_REG_FIFO, buffer, len);
}

void liblora_rf95_fifo_read(liblora_rf95_radio_t *radio, uint8_t *buffer, uint8_t len)
{
    // FIXME: try using liblora_driver_spi_read_burst
    for (int i = 0; i < len; i++)
        buffer[i] = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_FIFO);
}

// packet (private)
uint8_t liblora_rf95_packet_size(liblora_rf95_radio_t *radio)
{
    return liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_RX_NB_BYTES);
}

uint8_t liblora_rf95_packet_rssi(liblora_rf95_radio_t *radio)
{
    // FIXME: Fix correction based on freq
    int rssi_corr = 157;
    return liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_PKT_RSSI_VAL) - rssi_corr;
}

int8_t liblora_rf95_packet_snr(liblora_rf95_radio_t *radio)
{
    int8_t snr = (int8_t)(liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_PKT_SNR_VAL));
    return snr >> 2;
}

uint8_t liblora_rf95_packet_strength(liblora_rf95_radio_t *radio)
{
    int8_t snr = liblora_rf95_packet_snr(radio);
    if (snr < 0)
        return liblora_rf95_packet_rssi(radio) + snr;

    return liblora_rf95_packet_rssi(radio);
}

// packet (public)
liblora_rf95_packet_t liblora_rf95_packet_read(liblora_rf95_radio_t *radio)
{
    // FIXME: In Explicit Header Mode (default), use Crc from REG_HOP_CHANNEL

    liblora_rf95_packet_t pkt = {
        .buffer = {},
        .size = 0,
        .pkt_snr = 0,
        .pkt_rssi = 0,
        .rssi = 0,
        .strength = 0};
    uint8_t irq_flags = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS);
    if ((irq_flags & LIBLORA_RF95_IRQ_RX_DONE) != 0)
    {
        // clear RX_DONE IRQ flags
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS, LIBLORA_RF95_IRQ_RX_DONE | LIBLORA_RF95_IRQ_VALID_HEADER);

        // check IRQ flags
        if ((irq_flags & (LIBLORA_RF95_IRQ_RX_TIMEOUT | LIBLORA_RF95_IRQ_PAYLOAD_CRC_ERR)) != 0)
        {
            // TODO: Something went wrong
        }
        else
        {
            pkt.size = liblora_rf95_packet_size(radio);

            liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_FIFO_ADDR_PTR, liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_FIFO_RX_CURR_ADDR));
            liblora_rf95_fifo_read(radio, pkt.buffer, pkt.size);

            pkt.pkt_snr = liblora_rf95_packet_snr(radio);
            pkt.pkt_rssi = liblora_rf95_packet_rssi(radio);
            pkt.rssi = liblora_rf95_rssi(radio);
            pkt.strength = liblora_rf95_packet_strength(radio);
        }

        // reset FIFO addr to RX base addr & reset IRQ flags
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_FIFO_ADDR_PTR, liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_FIFO_RX_BASE_ADDR));
        liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS, irq_flags);
    }

    return pkt;
}

void liblora_rf95_packet_write(liblora_rf95_radio_t *radio, uint8_t *buffer, uint8_t len)
{
    // TODO: Logic in Explicit Header Mode (default)

    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS, 0xFF);
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_FIFO_ADDR_PTR, liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_FIFO_TX_BASE_ADDR));
    liblora_driver_spi_write(radio->driver, LIBLORA_RF95_REG_PAYLOAD_LENGTH, len);
    liblora_rf95_fifo_write(radio, buffer, len);
}

// other (public)
uint16_t liblora_rf95_valid_header_count(liblora_rf95_radio_t *radio)
{
    uint16_t count = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_RX_HEAD_CNT_VAL_MSB) << 8;
    count |= liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_RX_HEAD_CNT_VAL_LSB);

    return count;
}

uint16_t liblora_rf95_valid_packet_count(liblora_rf95_radio_t *radio)
{
    uint16_t count = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_RX_PKT_CNT_VAL_MSB) << 8;
    count |= liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_RX_PKT_CNT_VAL_LSB);

    return count;
}

int64_t liblora_rf95_frequency_error(liblora_rf95_radio_t *radio)
{
    // TODO: Fix this code
    // uint8_t fei_msb = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_FEI_MSB) & 0xF;

    // int32_t fei_val = (static_cast<int32_t>(fei_msb) & 0x7) << 16;
    // fei_val |= (static_cast<int32_t>(liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_FEI_MID)) << 8);
    // fei_val |= static_cast<int32_t>(liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_FEI_LSB));

    // Check sign bit
    // if (fei_msb & 0x8)
    //     fei_val -= 524288;

    // float ferr = ((static_cast<float>(fei_val) * (1L << 24)) / 32e6) * (this->get_bandwidth() / 500000.0f);
    // return static_cast<int64_t>(ferr);
    return 0;
}

uint8_t liblora_rf95_rssi(liblora_rf95_radio_t *radio)
{
    // FIXME: Fix correction based on freq
    int rssi_corr = 157;
    return liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_RSSI_VAL) - rssi_corr;
}

uint8_t liblora_rf95_random(liblora_rf95_radio_t *radio)
{
    return liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_RSSI_WIDEBAND);
}

liblora_rf95_modem_status_t liblora_rf95_modem_status(liblora_rf95_radio_t *radio)
{
    uint8_t curr = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_MODEM_STATUS);
    liblora_rf95_modem_status_t options[] = {
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
static liblora_rf95_radio_t* radios[64] = {NULL, };
static void liblora_rf95_handle_interrupt(int pin)
{
    liblora_rf95_radio_t* radio = radios[pin];
    if (radio == NULL)
        return;

    printf("liblora_rf95_handle_interrupt: interrupt received !\n");
    uint8_t irq_flags = liblora_driver_spi_read(radio->driver, LIBLORA_RF95_REG_IRQ_FLAGS);
    if ((irq_flags & LIBLORA_RF95_IRQ_RX_DONE) > 0)       // RX_DONE
    {
        liblora_rf95_packet_t pkt = liblora_rf95_packet_read(radio);
        printf("liblora_rf95_handle_interrupt: read packet !\n");
        if (radio->onrx != NULL) radio->onrx(pkt);
        printf("liblora_rf95_handle_interrupt: callback called !\n");
    }
    else if ((irq_flags & LIBLORA_RF95_IRQ_TX_DONE) > 0)  // TX_DONE
    {
        if (radio->ontx != NULL) radio->ontx();
    }
}

void liblora_rf95_onrx(liblora_rf95_radio_t *radio, void (*callback)(liblora_rf95_packet_t))
{
    radios[radio->dio0] = radio;
    radio->onrx = callback;
    liblora_driver_attach_interrupt(radio->driver, radio->dio0, INT_EDGE_RISING, liblora_rf95_handle_interrupt);
}

void liblora_rf95_ontx(liblora_rf95_radio_t *radio, void (*callback)(void))
{
    radios[radio->dio0] = radio;
    radio->ontx = callback;
    liblora_driver_attach_interrupt(radio->driver, radio->dio0, INT_EDGE_RISING, liblora_rf95_handle_interrupt);
}