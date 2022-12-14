from lora._liblora import (
    liblora_driver_wiringpi, 
    liblora_rf95_radio, 
    liblora_rf95_init,
    liblora_rf95_sleep,
    liblora_rf95_idle,
    liblora_rf95_recv,
    liblora_rf95_send,
    liblora_rf95_delete,
    liblora_rf95_packet_read,
    liblora_rf95_packet_write,
    liblora_rf95_valid_header_count,
    liblora_rf95_valid_packet_count,
    liblora_rf95_frequency_error,
    liblora_rf95_rssi,
    liblora_rf95_random,
    liblora_rf95_modem_status,
    liblora_rf95_onrx,
    liblora_rf95_ontx,
    liblora_rf95_config_frequency,
    liblora_rf95_config_bandwidth,
    liblora_rf95_config_spreading_factor,
    liblora_rf95_config_coding_rate,
    liblora_rf95_config_invert_iq,
    liblora_rf95_config_sync_word,
    LIBLORA_RF95_RX_CALLBACK,
    LIBLORA_RF95_TX_CALLBACK
)

class RF95:

    def __init__(self, spi_dev, spi_ss, dio0, rst):
        self.__spi_dev = spi_dev
        self.__spi_ss = spi_ss
        self.__dio0 = dio0
        self.__rst = rst

        self.__driver = None
        self.__radio = None

        self.__onrx = None
        self.__ontx = None

    def onrx(self, callback):
        liblora_rf95_onrx(self.__radio, LIBLORA_RF95_RX_CALLBACK(callback))

    def ontx(self, callback):
        liblora_rf95_ontx(self.__radio, LIBLORA_RF95_TX_CALLBACK(callback))

    def __del__(self):
        liblora_rf95_sleep(self.__radio)
        return liblora_rf95_delete(self.__radio)

    def init(self, frequency, sf, bw):
        self.__driver = liblora_driver_wiringpi(self.__spi_dev, self.__spi_ss)
        if self.__driver is None:
            raise RuntimeError('Failed to create wiringPi driver')
        
        self.__radio = liblora_rf95_radio(self.__driver, self.__dio0, self.__rst)
        if self.__radio is None:
            raise RuntimeError('Failed to create radio')

        # freq = 868100000 # 8681E5
        if liblora_rf95_init(self.__radio, frequency, sf, bw) != 0:
            raise RuntimeError('Failed to initialize radio')


    # Config
    def config_frequency(self, frequency):
        liblora_rf95_config_frequency(self.__radio, frequency)

    def config_bandwidth(self, bw, optimize):
        liblora_rf95_config_bandwidth(self.__radio, bw, optimize)

    def config_spreading_factor(self, sf, optimize):
        liblora_rf95_config_spreading_factor(self.__radio, sf, optimize)

    def config_coding_rate(self, cr):
        liblora_rf95_config_coding_rate(self.__radio, cr)

    def config_invert_iq(self, enable, rx):
        liblora_rf95_config_invert_iq(self.__radio, enable, rx)
    
    def config_sync_word(self, sync_word):
        liblora_rf95_config_sync_word(self.__radio, sync_word)


    # Opmode
    def sleep(self):
        liblora_rf95_sleep(self.__radio)

    def idle(self):
        liblora_rf95_idle(self.__radio)

    def recv(self, continuous):
        return liblora_rf95_recv(self.__radio, continuous)

    def send(self, _async):
        liblora_rf95_send(self.__radio, _async)


    # Packets
    def read_pkt(self):
        return liblora_rf95_packet_read(self.__radio)

    def write_pkt(self, _bytes):
        return liblora_rf95_packet_write(self.__radio, _bytes, len(_bytes))


    # Other
    @property
    def valid_header_cnt(self):
        return liblora_rf95_valid_header_count(self.__radio)

    @property
    def valid_packet_cnt(self):
        return liblora_rf95_valid_packet_count(self.__radio)

    @property
    def frequency_error(self):
        return liblora_rf95_frequency_error(self.__radio)

    @property
    def rssi(self):
        return liblora_rf95_rssi(self.__radio)

    @property
    def random(self):
        return liblora_rf95_random(self.__radio)

    @property
    def modem_status(self):
        return liblora_rf95_modem_status(self.__radio)
