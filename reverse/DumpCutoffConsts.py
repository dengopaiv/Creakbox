# @category Creakbox
# @runtime Jython

from ghidra.program.model.data import FloatDataType, DoubleDataType
import jarray
import struct

program = currentProgram
mem = program.getMemory()
af = program.getAddressFactory().getDefaultAddressSpace()

# 4 constants in getCutoff + 1 shared denominator in other getters
ADDRS = {
    "_DAT_0045d3c4 (Volume divisor)": 0x0045d3c4,
    "_DAT_0045d528 (Tune divisor)":   0x0045d528,
    "_DAT_0045d6ec (Cutoff C1)":      0x0045d6ec,
    "_DAT_0045d6f0 (Cutoff C2)":      0x0045d6f0,
    "_DAT_0045d6f4 (Cutoff C3)":      0x0045d6f4,
    "_DAT_0045d6f8 (Cutoff C4)":      0x0045d6f8,
    "_DAT_0045d864 (Rezo divisor)":   0x0045d864,
    "_DAT_0045d9c0 (EnvMod divisor)": 0x0045d9c0,
    "_DAT_0045db4c (Decay divisor)":  0x0045db4c,
    "_DAT_0045e598 (breakpoint div)": 0x0045e598,
}

def read_extended80(addr_val):
    """Read 10-byte x87 extended float at address, return as double."""
    buf = jarray.zeros(10, "b")
    try:
        mem.getBytes(af.getAddress(long(hex(addr_val), 16)), buf)
    except Exception as e:
        return None, "err " + str(e)
    # buf holds 10 bytes of x87 extended precision
    mantissa = 0
    for i in range(8):
        mantissa |= (buf[i] & 0xFF) << (i * 8)
    se = ((buf[8] & 0xFF) | ((buf[9] & 0xFF) << 8))
    sign = se >> 15
    exp = se & 0x7FFF
    if exp == 0 and mantissa == 0:
        return -0.0 if sign else 0.0, "zero"
    if exp == 0x7FFF:
        return float('nan'), "inf/nan"
    # Normalized: value = (-1)^sign * (mantissa/2^63) * 2^(exp-16383)
    frac = float(mantissa) / float(1 << 63)
    value = frac * (2.0 ** (exp - 16383))
    if sign: value = -value
    hex_bytes = ' '.join('%02X' % (buf[i] & 0xFF) for i in range(10))
    return value, hex_bytes

def read_float4(addr_val):
    """Read 4-byte float."""
    buf = jarray.zeros(4, "b")
    try:
        mem.getBytes(af.getAddress(long(hex(addr_val), 16)), buf)
    except Exception as e:
        return None, "err " + str(e)
    import array
    ba = [(buf[i] & 0xFF) for i in range(4)]
    raw = bytes(bytearray(ba))
    (val,) = struct.unpack('<f', raw)
    hex_bytes = ' '.join('%02X' % b for b in ba)
    return val, hex_bytes

def read_double8(addr_val):
    """Read 8-byte double."""
    buf = jarray.zeros(8, "b")
    try:
        mem.getBytes(af.getAddress(long(hex(addr_val), 16)), buf)
    except Exception as e:
        return None, "err " + str(e)
    ba = [(buf[i] & 0xFF) for i in range(8)]
    raw = bytes(bytearray(ba))
    (val,) = struct.unpack('<d', raw)
    hex_bytes = ' '.join('%02X' % b for b in ba)
    return val, hex_bytes

print("Delphi compilers use extended float10 (80-bit) for literals by default.")
print("Trying all three widths for each constant:\n")
for name, a in sorted(ADDRS.items(), key=lambda x: x[1]):
    v4, h4   = read_float4(a)
    v8, h8   = read_double8(a)
    v10, h10 = read_extended80(a)
    print("  %-40s  0x%08X" % (name, a))
    print("    as float4:    %s" % (v4,))
    print("    as double8:   %s" % (v8,))
    print("    as extended80: %s  (bytes: %s)" % (v10, h10))
