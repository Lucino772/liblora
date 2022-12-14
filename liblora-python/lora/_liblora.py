import ctypes
import ctypes.util

from enum import IntEnum

liblora_so = ctypes.util.find_library('lora')
liblora = ctypes.CDLL(liblora_so)

# Helpers
class CtypesEnum(IntEnum):
    """A ctypes-compatible IntEnum superclass."""
    @classmethod
    def from_param(cls, obj):
        return int(obj)

BUFFER = ctypes.POINTER(ctypes.c_uint8)

# Structures & Enums
class liblora_rf95_packet_t(ctypes.Structure):
    _fields_ = [
        ('buffer', ctypes.c_uint8 * 256),
        ('size', ctypes.c_uint8),
        ('pkt_snr', ctypes.c_int8),
        ('pkt_rssi', ctypes.c_uint8),
        ('rssi', ctypes.c_uint8),
        ('strength', ctypes.c_uint8)
    ]

class liblora_driver_t(ctypes.Structure):
    _fields_ = [
        ('config', ctypes.c_void_p),

        # GPIO
        ('setup', ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p)),
        ('pin_mode', ctypes.CFUNCTYPE(None, ctypes.c_int, ctypes.c_int, ctypes.c_void_p)),
        ('pin_write', ctypes.CFUNCTYPE(None, ctypes.c_int, ctypes.c_int, ctypes.c_void_p)),
        ('pin_read', ctypes.CFUNCTYPE(ctypes.c_uint8, ctypes.c_int, ctypes.c_void_p)),
        ('delay', ctypes.CFUNCTYPE(None, ctypes.c_int, ctypes.c_void_p)),

        # SPI
        ('spi_init', ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p)),
        ('spi_read', ctypes.CFUNCTYPE(ctypes.c_uint8, ctypes.c_void_p)),
        ('spi_read_burst', ctypes.CFUNCTYPE(ctypes.c_uint8, ctypes.c_uint8, BUFFER, ctypes.c_int, ctypes.c_void_p)),
        ('spi_write', ctypes.CFUNCTYPE(None, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_void_p)),
        ('spi_write_burst', ctypes.CFUNCTYPE(None, ctypes.c_uint8, BUFFER, ctypes.c_int, ctypes.c_void_p)),

        # Interrupts
        ('attach_interrupt', ctypes.CFUNCTYPE(
                None, 
                ctypes.c_int, 
                ctypes.c_int,
                ctypes.CFUNCTYPE(None, ctypes.c_int),
                ctypes.c_void_p
            )
        )
    ]

liblora_driver_t_p = ctypes.POINTER(liblora_driver_t)

class liblora_rf95_radio_t(ctypes.Structure):
    _fields_ = [
        ('driver', liblora_driver_t_p),
        ('dio0', ctypes.c_int),
        ('rst', ctypes.c_int),

        # Callbacks
        ('onrx', ctypes.CFUNCTYPE(None, liblora_rf95_packet_t)),
        ('ontx', ctypes.CFUNCTYPE(None))
    ]

liblora_rf95_radio_t_p = ctypes.POINTER(liblora_rf95_radio_t)

class liblora_rf95_modem_status_t(CtypesEnum):
    UNKNOWN                 = 0x0
    SIGNAL_DETECTED         = 0x1
    SIGNAL_SYNCED           = 0x2
    RX_ON_GOING             = 0x4
    HEADER_INFO_VALID       = 0x8
    MODEM_CLEAR             = 0x10


# Functions (rf95)
## Init
liblora_rf95_radio = liblora.liblora_rf95_radio
liblora_rf95_radio.argtypes = [liblora_driver_t_p, ctypes.c_int, ctypes.c_int]
liblora_rf95_radio.restype = liblora_rf95_radio_t_p

liblora_rf95_init = liblora.liblora_rf95_init
liblora_rf95_init.argtypes = [
    liblora_rf95_radio_t_p, ctypes.c_long, ctypes.c_uint8, ctypes.c_uint8
]
liblora_rf95_init.restype = ctypes.c_int

liblora_rf95_delete = liblora.liblora_rf95_delete
liblora_rf95_delete.argtypes = [liblora_rf95_radio_t_p]
liblora_rf95_delete.restype = None

## opmode
liblora_rf95_sleep = liblora.liblora_rf95_sleep
liblora_rf95_sleep.argtypes = [liblora_rf95_radio_t_p]
liblora_rf95_sleep.restype = None

liblora_rf95_idle = liblora.liblora_rf95_idle
liblora_rf95_idle.argtypes = [liblora_rf95_radio_t_p]
liblora_rf95_idle.restype = None

liblora_rf95_recv = liblora.liblora_rf95_recv
liblora_rf95_recv.argtypes = [liblora_rf95_radio_t_p, ctypes.c_bool]
liblora_rf95_recv.restype = ctypes.c_bool

liblora_rf95_send = liblora.liblora_rf95_send
liblora_rf95_send.argtypes = [liblora_rf95_radio_t_p, ctypes.c_bool]
liblora_rf95_send.restype = None

## config
liblora_rf95_config_frequency = liblora.liblora_rf95_config_frequency
liblora_rf95_config_frequency.argtypes = [liblora_rf95_radio_t_p, ctypes.c_long]
liblora_rf95_config_frequency.restype = None

liblora_rf95_config_bandwidth = liblora.liblora_rf95_config_bandwidth
liblora_rf95_config_bandwidth.argtypes = [liblora_rf95_radio_t_p, ctypes.c_uint8, ctypes.c_bool]
liblora_rf95_config_bandwidth.restype = None

liblora_rf95_config_spreading_factor = liblora.liblora_rf95_config_spreading_factor
liblora_rf95_config_spreading_factor.argtypes = [liblora_rf95_radio_t_p, ctypes.c_uint8, ctypes.c_bool]
liblora_rf95_config_spreading_factor.restype = None

liblora_rf95_config_coding_rate = liblora.liblora_rf95_config_coding_rate
liblora_rf95_config_coding_rate.argtypes = [liblora_rf95_radio_t_p, ctypes.c_uint8]
liblora_rf95_config_coding_rate.restype = None

liblora_rf95_config_invert_iq = liblora.liblora_rf95_config_invert_iq
liblora_rf95_config_invert_iq.argtypes = [liblora_rf95_radio_t_p, ctypes.c_bool, ctypes.c_bool]
liblora_rf95_config_invert_iq.restype = None

liblora_rf95_config_sync_word = liblora.liblora_rf95_config_sync_word
liblora_rf95_config_sync_word.argtypes = [liblora_rf95_radio_t_p, ctypes.c_uint8]
liblora_rf95_config_sync_word.restype = None

## packet
liblora_rf95_packet_read = liblora.liblora_rf95_packet_read
liblora_rf95_packet_read.argtypes = [liblora_rf95_radio_t_p]
liblora_rf95_packet_read.restype = liblora_rf95_packet_t

liblora_rf95_packet_write = liblora.liblora_rf95_packet_write
liblora_rf95_packet_write.argtypes = [liblora_rf95_radio_t_p, BUFFER, ctypes.c_uint8]
liblora_rf95_packet_write.restype = None

## other
liblora_rf95_valid_header_count = liblora.liblora_rf95_valid_header_count
liblora_rf95_valid_header_count.argtypes = [liblora_rf95_radio_t_p]
liblora_rf95_valid_header_count = ctypes.c_uint16

liblora_rf95_valid_packet_count = liblora.liblora_rf95_valid_packet_count
liblora_rf95_valid_packet_count.argtypes = [liblora_rf95_radio_t_p]
liblora_rf95_valid_packet_count = ctypes.c_uint16

liblora_rf95_frequency_error = liblora.liblora_rf95_frequency_error
liblora_rf95_frequency_error.argtypes = [liblora_rf95_radio_t_p]
liblora_rf95_frequency_error = ctypes.c_int64

liblora_rf95_rssi = liblora.liblora_rf95_rssi
liblora_rf95_rssi.argtypes = [liblora_rf95_radio_t_p]
liblora_rf95_rssi = ctypes.c_uint8

liblora_rf95_random = liblora.liblora_rf95_random
liblora_rf95_random.argtypes = [liblora_rf95_radio_t_p]
liblora_rf95_random = ctypes.c_uint8

liblora_rf95_modem_status = liblora.liblora_rf95_modem_status
liblora_rf95_modem_status.argtypes = [liblora_rf95_radio_t_p]
liblora_rf95_modem_status = liblora_rf95_modem_status_t

## interrupts
LIBLORA_RF95_RX_CALLBACK = ctypes.CFUNCTYPE(None, liblora_rf95_packet_t)
liblora_rf95_onrx = liblora.liblora_rf95_onrx
liblora_rf95_onrx.argtypes = [liblora_rf95_radio_t_p, LIBLORA_RF95_RX_CALLBACK]
liblora_rf95_onrx.restype = None

LIBLORA_RF95_TX_CALLBACK = ctypes.CFUNCTYPE(None)
liblora_rf95_ontx = liblora.liblora_rf95_ontx
liblora_rf95_ontx.argtypes = [liblora_rf95_radio_t_p, LIBLORA_RF95_TX_CALLBACK]
liblora_rf95_ontx.restype = None

# Functions (drivers)
liblora_driver_wiringpi = liblora.liblora_driver_wiringpi
liblora_driver_wiringpi.argtypes = [ctypes.c_int, ctypes.c_int]
liblora_driver_wiringpi.restype = liblora_driver_t_p

liblora_driver_delete = liblora.liblora_driver_delete
liblora_driver_delete.argtypes = [liblora_driver_t_p]
liblora_driver_delete.restype = None
