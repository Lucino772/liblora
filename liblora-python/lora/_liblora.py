import os

import ctypes
import ctypes.util

os.environ['LIBRARY_PATH'] = os.environ['LD_LIBRARY_PATH']

liblora_so = ctypes.util.find_library('lora')
liblora = ctypes.CDLL(liblora_so)
