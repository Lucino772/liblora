import math
from fractions import Fraction

# sf = Spreading Factor (7 -> 12)
# bw = Bandwidth (125, 250, 500)
# np = Preamble Len
# pl = Payload Len
# crc = 1 if crc else 0
# ih = 1 if implicite header else 0
# de = 1 if LowDataRateOptimize else 0
# cr = 1 if 4/5, 4 if 4/8

Rs = lambda sf, bw: Fraction(bw, (2**sf))           # Symbol Rate
Ts = lambda sf, bw: Fraction(1, Rs(sf, bw))         # Symbol Length
Duration = lambda nsym, sf, bw: Fraction(nsym * Ts(sf, bw))   # Duration (ms)
BitRate = lambda sf, cr, bw: 1000 * sf * Fraction(Fraction(4, 4 + cr), Fraction(2**sf, bw))

# Preamble Length/Duration (ms)
Tpreamble = lambda sf, bw, np: Duration(np + 4.25, sf, bw)

# Nb Payload Symbols
Npayload = lambda pl, sf, crc, ih, de, cr: 8 + max(math.ceil( Fraction(8*pl - 4*sf + 28 + 16 * crc - 20 * ih, (4 * (sf - 2*de))) ) * (cr + 4), 0)
# Payload Length/Duration (ms)
Tpayload = lambda sf, bw, pl, crc, ih, de, cr: Duration(Npayload(pl, sf, crc, ih, de, cr), sf, bw)

# TimeOnAir (ms)
TimeOnAir = lambda sf, bw, np, pl, crc, ih, de, cr: Tpreamble(sf, bw, np) + Tpayload(sf, bw, pl, crc, ih, de, cr)


DutyCycle = lambda perc, toa: Fraction(Fraction((1 - perc) * 100) * toa, 1000)      # DutyCycle (s)
MessagePerHour = lambda perc, toa: Fraction(3600, DutyCycle(perc, toa))

for sf in range(7, 8):
    toa = 0
    if sf <= 10:
        toa = TimeOnAir(sf, 125, 8, 234, 1, 0, 0, 1)
    else:
        toa = TimeOnAir(sf, 125, 8, 234, 1, 0, 1, 1)

    print('BitRate', float(BitRate(sf, 1, 125)))
    print('SymbLen', float(Ts(sf, 125)))
    print('PreamLen', float(Tpreamble(sf, 125, 8)))
    print('Toa', float(toa))
    print('DutyCycle', float(DutyCycle(0.01, toa)))
    print('Message/H', float(MessagePerHour(0.01, toa)))
