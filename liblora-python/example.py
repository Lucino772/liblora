# from lora import _liblora
from lora.rf95 import RF95

radio = RF95(0, 6, 7, 6)

def on_receive(pkt):
    if pkt.size > 0:
        data = bytes(pkt.buffer[:pkt.size]).decode('utf-8')
        print("Packet received ({} bytes): {}".format(pkt.size, data))
    else:
        print("Something went wrong !")

def main():
    try:
        radio.init(868100000, 0x07, 0x07)
    except RuntimeError:
        return

    radio.onrx(on_receive)
    if not radio.recv(True):
        print("Failed to put in RX_CONT mode")
        return

    try:
        while True:
            pass
    except KeyboardInterrupt:
        pass

    radio.sleep()


if __name__ == '__main__':
    main()