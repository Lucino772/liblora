from lora import _liblora

def on_receive(pkt):
    if pkt.size > 0:
        print("Packet received ({} bytes): {}".format(pkt.size, bytearray(pkt.buffer)))
    else:
        print("Something went wrong !")

def main():
    driver = _liblora.liblora_driver_wiringpi(0, 6)
    if driver is None:
        print('Failed to create wiringPi driver')
        return 1
    
    radio = _liblora.liblora_rf95_radio(driver, 7, 6)
    if radio is None:
        print('Failed to create radio')
        return 1

    freq = 868100000 # 8681E5
    if _liblora.liblora_rf95_init(radio, freq, 0x07, 0x07) != 0:
        print('Failed to initialize radio')
        return 1

    _liblora.liblora_rf95_onrx(radio, _liblora.LIBLORA_RF95_RX_CALLBACK(on_receive))

    if not _liblora.liblora_rf95_recv(radio, True):
        print("Failed to put in RX_CONT mode")

    try:
        while True:
            pass
    except KeyboardInterrupt:
        pass

    _liblora.liblora_rf95_sleep(radio)
    _liblora.liblora_rf95_delete(radio)


if __name__ == '__main__':
    main()