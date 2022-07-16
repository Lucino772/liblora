#include "./driver.h"
#include <wiringPi.h>
#include "./registers.h"
#include "../spi.h"

// Initialisation
sx127x_dev_t sx127x_create(int spi_dev, int spi_ss, int dio0, int rst)
{
    wiringPiSetup(); // FIXME: This class should not interfere with wiringPi (abstract wiringPi)

    return sx127x_dev_t{
        spi_dev,
        spi_ss,
        dio0,
        rst};
}

int sx127x_init(sx127x_dev_t dev, long freq, uint8_t sf, uint8_t bw)
{
    pinMode(dev.dio0, INPUT);
    pinMode(dev.rst, OUTPUT);

    // Reset chip
    if (dev.rst != -1)
    {
        digitalWrite(dev.rst, LOW);
        delay(100);
        digitalWrite(dev.rst, HIGH);
        delay(100);
    }

    if (spi_driver_init(dev.spi_dev, dev.spi_ss, 500000) == -1)
        return -1;

    // check version
    uint8_t ver = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_VERSION);
    if (ver != 0x12)
        return -1;

    // sleep & lora
    sx127x_opmode_t curr_opmode = sx127x_opmode_read(dev);
    curr_opmode.lora = true;
    curr_opmode.mode = SX127x_OPMODE_SLEEP;
    sx127x_opmode_write(dev, curr_opmode);

    // Configure
    sx127x_config_frequency(dev, freq);
    sx127x_config_bandwidth(dev, bw, true);
    sx127x_config_spreading_factor(dev, sf, true);
    sx127x_config_pa(dev, true, 17, 4);
    sx127x_config_lna(dev, true, 0);
    sx127x_config_agc(dev, true);

    // Reset fifo addresses
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_FIFO_TX_BASE_ADDR, 0x00);
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_FIFO_RX_BASE_ADDR, 0x00);

    sx127x_sleep(dev);
    return 0;
}

// opmode (private)
sx127x_opmode_t sx127x_opmode_read(sx127x_dev_t dev)
{
    uint8_t _opmode = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_OPMODE);
    return sx127x_opmode_t{
        (_opmode & 0x80) == 0x80, // lora mode
        (_opmode & 0x40) == 0x40, // shared reg
        (_opmode & 0x04) == 0x04, // low freq. on
        (uint8_t)(_opmode | 0x07)};
}

void sx127x_opmode_write(sx127x_dev_t dev, sx127x_opmode_t _opmode)
{
    uint8_t _new = _opmode.mode;
    if (_opmode.lora)
        _new |= 0x80;
    if (_opmode.shared_reg)
        _new |= 0x40;
    if (_opmode.low_freq)
        _new |= 0x04;

    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_OPMODE, _new);
}

// opmode (public)
void sx127x_sleep(sx127x_dev_t dev)
{
    sx127x_opmode_t curr = sx127x_opmode_read(dev);
    curr.mode = SX127x_OPMODE_SLEEP;
    sx127x_opmode_write(dev, curr);
}

void sx127x_idle(sx127x_dev_t dev)
{
    sx127x_opmode_t curr = sx127x_opmode_read(dev);
    curr.mode = SX127x_OPMODE_STDBY;
    sx127x_opmode_write(dev, curr);
}

bool sx127x_receive(sx127x_dev_t dev, bool continuous = false)
{
    // TODO: Check if transmitting, if so return false

    uint8_t _mode = continuous ? SX127x_OPMODE_RX_CONT : SX127x_OPMODE_RX_SINGLE;
    if (sx127x_opmode_read(dev).mode != _mode)
    {
        // DIO0=RXDONE, DIO1=NOP, DIO2=NOP, DIO3=NOP
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_DIO_MAPPING_1, SX127x_DIO0_RX_DONE | SX127x_DIO1_NOP | SX127x_DIO2_NOP | SX127x_DIO3_NOP);

        // Reset IRQ flags and set IRQ flags mask
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS, 0xFF);
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS_MASK, SX127x_IRQ_TX_DONE | SX127x_IRQ_CAD_DONE | SX127x_IRQ_FHSS_CHANGE_CHAN | SX127x_IRQ_CAD_DETECTED);

        // Change OpMode
        sx127x_opmode_t curr = sx127x_opmode_read(dev);
        curr.mode = _mode;
        sx127x_opmode_write(dev, curr);
    }

    return true;
}

sx127x_packet_t sx127x_get_packet(sx127x_dev_t dev)
{
    // Try read packet
    sx127x_packet_t pkt = sx127x_read_packet(dev);

    // Only for RX_SINGLE mode: reset DIO mapping
    // & IRQ mask (if packet was received) and go
    // back to STANDBY mode
    if (sx127x_opmode_read(dev).mode == SX127x_OPMODE_RX_SINGLE && pkt.size > 0)
    {
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS_MASK, 0xFF);
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_DIO_MAPPING_1, 0xFF);
        sx127x_idle(dev);
    }

    return pkt;
}

void sx127x_send(sx127x_dev_t dev, uint8_t *buffer, uint8_t len, bool async = false)
{
    // DIO0=TXDONE, DIO1=NOP, DIO2=NOP, DIO3=NOP
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_DIO_MAPPING_1, SX127x_DIO0_TX_DONE | SX127x_DIO1_NOP | SX127x_DIO2_NOP | SX127x_DIO3_NOP);

    // Reset IRQ flags and set IRQ flags mask
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS, 0xFF);
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS_MASK, ~(SX127x_IRQ_TX_DONE));

    sx127x_write_packet(dev, buffer, len);

    // OPMODE_TX: Transmit packet
    sx127x_opmode_t curr = sx127x_opmode_read(dev);
    curr.mode = SX127x_OPMODE_TX;
    sx127x_opmode_write(dev, curr);

    // wait for TxDone
    if (!async)
        while ((spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS) & SX127x_IRQ_TX_DONE) == 0)
            ;

    // reset DIO mapping & IRQ mask
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS_MASK, 0xFF);
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_DIO_MAPPING_1, 0xFF);
}

// config (public)
void sx127x_config_frequency(sx127x_dev_t dev, long freq)
{
    uint64_t frf = ((uint64_t)freq << 19) / 32000000;
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_FR_MSB, (uint8_t)(frf >> 16));
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_FR_MID, (uint8_t)(frf >> 8));
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_FR_LSB, (uint8_t)(frf >> 0));
}

void sx127x_config_bandwidth(sx127x_dev_t dev, uint8_t bw, bool optimize = true)
{
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG1);
    uint8_t _new = (bw << 4) | (curr & 0xF);
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG1, _new);

    if (optimize && bw == SX127x_BW_7_8)
        sx127x_config_high_bw_optimization(dev, true);
    else
        sx127x_config_high_bw_optimization(dev, false);
}

void sx127x_config_spreading_factor(sx127x_dev_t dev, uint8_t sf, bool optimize = true)
{
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG2);
    uint8_t _new = (sf << 4) | (curr & 0xF);

    if (sf == SX127x_SF_6)
    {
        sx127x_config_detection_optimization(dev, 0x05);
        sx127x_config_detection_threshold(dev, 0x0C);
    }
    else
    {
        sx127x_config_detection_optimization(dev, 0x03);
        sx127x_config_detection_optimization(dev, 0xA);
    }

    if (sf == SX127x_SF_11 || sf == SX127x_SF_12)
        sx127x_config_low_data_rate_optimization(dev, true);
    else
        sx127x_config_low_data_rate_optimization(dev, false);

    // TODO: understand why ?
    // if (sf == sf_t::SF10 || sf == sf_t::SF11 || sf == sf_t::SF12) {
    //     spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_SYMB_TIMEOUT_LSB, 0x05);
    // } else {
    //     spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_SYMB_TIMEOUT_LSB, 0x08);
    // }
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG2, _new);
}

void sx127x_config_coding_rate(sx127x_dev_t dev, uint8_t cr)
{
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG1);
    uint8_t _new = (curr & 0xF1) | (curr << 1);
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG1, _new);
}

void sx127x_config_invert_iq(sx127x_dev_t dev, bool enable, bool rx = true)
{
    // TODO: Check if it's correct
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_INVERT_IQ);
    if (enable)
    {
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_INVERT_IQ, (curr & ~0x41) | 0x40);
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_INVERT_IQ2, 0x19);
    }
    else
    {
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_INVERT_IQ, (curr & ~0x41) | 0x01);
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_INVERT_IQ2, 0x1D);
    }
}

void sx127x_config_sync_word(sx127x_dev_t dev, uint8_t value)
{
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_SYNC_WORD, value);
}

// config/optimization (private)
void sx127x_config_low_data_rate_optimization(sx127x_dev_t dev, bool enable)
{
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG3);
    uint8_t _new = curr & ~0x8;
    if (enable)
        _new |= 0x8;

    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG3, _new);
}

void sx127x_config_detection_optimization(sx127x_dev_t dev, uint8_t value)
{
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_DETECT_OPTI, value);
}

void sx127x_config_detection_threshold(sx127x_dev_t dev, uint8_t value)
{
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_DETECT_THRESHOLD, value);
}

void sx127x_config_high_bw_optimization(sx127x_dev_t dev, bool enable)
{
    if (enable)
    {
        // FIX: Values differs for frequencies
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_HIGH_BW_OPTI1, 0x02);
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_HIGH_BW_OPTI2, 0x64);
    }
    else
    {
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_HIGH_BW_OPTI1, 0x03);

        // Automatically reset by the chip
        // spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_HIGH_BW_OPTI2, 0x65);
    }
}

// config (private)
void sx127x_config_crc(sx127x_dev_t dev, bool enable)
{
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG2);
    uint8_t _new = curr & ~0x04;
    if (enable)
        _new |= 0x04;

    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG2, _new);
}

void sx127x_config_header_mode(sx127x_dev_t dev, bool _explicit)
{
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG1);
    uint8_t _new = curr & ~0x01;
    if (!_explicit)
        _new |= 0x01;

    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG1, _new);
}

void sx127x_config_symb_timeout(sx127x_dev_t dev, uint16_t timeout)
{
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG2);
    uint8_t _new = (curr & ~0x3) | ((timeout >> 16) & 0x3);
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG2, _new);
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_SYMB_TIMEOUT_LSB, timeout & 0xFF);
}

void sx127x_config_preamble_len(sx127x_dev_t dev, uint16_t len)
{
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_PREAMBLE_MSB, len >> 8);
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_PREAMBLE_LSB, len & 0xFF);
}

void sx127x_config_max_payload_len(sx127x_dev_t dev, uint8_t len)
{
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_MAX_PAYLOAD_LENGTH, len);
}

void sx127x_config_hop_period(sx127x_dev_t dev, uint8_t value)
{
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_HOP_PERIOD, value);
}

void sx127x_config_pa(sx127x_dev_t dev, bool boost, uint8_t power, uint8_t max_power)
{
    uint8_t _new = 0;
    if (boost)
        _new |= 0x80;
    _new |= (max_power & 0x7) << 4;
    _new |= (power - 2) & 0xF;

    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_PA_CONFIG, _new);
}

void sx127x_config_agc(sx127x_dev_t dev, bool enable)
{
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG3);
    uint8_t _new = (curr & ~0x4);
    if (enable)
        _new |= 0x4;

    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_CONFIG3, _new);
}

void sx127x_config_pa_ramp(sx127x_dev_t dev, uint8_t ramp)
{
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_PA_RAMP);
    uint8_t _new = (curr & 0xF) | (ramp & 0xF);
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_PA_RAMP, _new);
}

void sx127x_config_ocp(sx127x_dev_t dev, bool enable, uint8_t trim)
{
    uint8_t _new = 0;
    if (enable)
        _new |= 0x10;
    _new |= trim & 0xF;

    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_OCP, _new);
}

void sx127x_config_lna(sx127x_dev_t dev, bool boost, uint8_t gain)
{
    uint8_t _new = 0;
    if (boost)
        _new |= 0x03;
    _new |= gain << 5;

    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_LNA, _new);
}

// fifo (private)
void sx127x_fifo_write(sx127x_dev_t dev, uint8_t *buffer, uint8_t len)
{
    spi_write_burst(dev.spi_dev, dev.spi_ss, SX127x_REG_FIFO, buffer, len);
}

void sx127x_fifo_read(sx127x_dev_t dev, uint8_t *buffer, uint8_t len)
{
    // FIXME: try using spi_read_burst
    for (int i = 0; i < len; i++)
        buffer[i] = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_FIFO);
}

// packet (private)
uint8_t sx127x_packet_size(sx127x_dev_t dev)
{
    return spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_RX_NB_BYTES);
}

uint8_t sx127x_packet_rssi(sx127x_dev_t dev)
{
    // FIXME: Fix correction based on freq
    int rssi_corr = 157;
    return spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_PKT_RSSI_VAL) - rssi_corr;
}

int8_t sx127x_packet_snr(sx127x_dev_t dev)
{
    int8_t snr = (int8_t)(spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_PKT_SNR_VAL));
    return snr >> 2;
}

uint8_t sx127x_packet_strength(sx127x_dev_t dev)
{
    int8_t snr = sx127x_packet_snr(dev);
    if (snr < 0)
        return sx127x_packet_rssi(dev) + snr;

    return sx127x_packet_rssi(dev);
}

sx127x_packet_t sx127x_read_packet(sx127x_dev_t dev)
{
    // FIXME: In Explicit Header Mode (default), use Crc from REG_HOP_CHANNEL

    sx127x_packet_t pkt{{}, 0, 0, 0, 0};
    uint8_t irq_flags = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS);
    if ((irq_flags & SX127x_IRQ_RX_DONE) != 0)
    {
        // clear RX_DONE IRQ flags
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS, SX127x_IRQ_RX_DONE | SX127x_IRQ_VALID_HEADER);

        // check IRQ flags
        if ((irq_flags & (SX127x_IRQ_RX_TIMEOUT | SX127x_IRQ_PAYLOAD_CRC_ERR)) != 0)
        {
            // TODO: Something went wrong
        }
        else
        {
            pkt.size = sx127x_packet_size(dev);

            spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_FIFO_ADDR_PTR, spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_FIFO_RX_CURR_ADDR));
            sx127x_fifo_read(dev, pkt.buffer, pkt.size);

            pkt.pkt_snr = sx127x_packet_snr(dev);
            pkt.pkt_rssi = sx127x_packet_rssi(dev);
            pkt.rssi = sx127x_rssi(dev);
            pkt.strength = sx127x_packet_strength(dev);
        }

        // reset FIFO addr to RX base addr & reset IRQ flags
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_FIFO_ADDR_PTR, spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_FIFO_RX_BASE_ADDR));
        spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS, irq_flags);
    }

    return pkt;
}

void sx127x_write_packet(sx127x_dev_t dev, uint8_t *buffer, uint8_t len)
{
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_IRQ_FLAGS, 0xFF);
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_FIFO_ADDR_PTR, spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_FIFO_TX_BASE_ADDR));
    spi_write(dev.spi_dev, dev.spi_ss, SX127x_REG_PAYLOAD_LENGTH, len);
    sx127x_fifo_write(dev, buffer, len);
}

// other (public)
uint16_t sx127x_valid_header_count(sx127x_dev_t dev)
{
    uint16_t count = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_RX_HEAD_CNT_VAL_MSB) << 8;
    count |= spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_RX_HEAD_CNT_VAL_LSB);

    return count;
}

uint16_t sx127x_valid_packet_count(sx127x_dev_t dev)
{
    uint16_t count = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_RX_PKT_CNT_VAL_MSB) << 8;
    count |= spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_RX_PKT_CNT_VAL_LSB);

    return count;
}

int64_t sx127x_frequency_error(sx127x_dev_t dev)
{
    // TODO: Fix this code
    // uint8_t fei_msb = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_FEI_MSB) & 0xF;

    // int32_t fei_val = (static_cast<int32_t>(fei_msb) & 0x7) << 16;
    // fei_val |= (static_cast<int32_t>(spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_FEI_MID)) << 8);
    // fei_val |= static_cast<int32_t>(spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_FEI_LSB));

    // // Check sign bit
    // if (fei_msb & 0x8)
    //     fei_val -= 524288;

    // float ferr = ((static_cast<float>(fei_val) * (1L << 24)) / 32e6) * (this->get_bandwidth() / 500000.0f);
    // return static_cast<int64_t>(ferr);
    return 0;
}

uint8_t sx127x_rssi(sx127x_dev_t dev)
{
    // FIXME: Fix correction based on freq
    int rssi_corr = 157;
    return spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_RSSI_VAL) - rssi_corr;
}

uint8_t sx127x_random(sx127x_dev_t dev)
{
    return spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_RSSI_WIDEBAND);
}

sx127x_modem_status_t sx127x_modem_status(sx127x_dev_t dev)
{
    uint8_t curr = spi_read(dev.spi_dev, dev.spi_ss, SX127x_REG_MODEM_STATUS);
    sx127x_modem_status_t options[] = {
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
