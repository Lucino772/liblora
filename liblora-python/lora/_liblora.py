import ctypes
import ctypes.util

liblora_so = ctypes.util.find_library('lora')
liblora = ctypes.CDLL(liblora_so)
