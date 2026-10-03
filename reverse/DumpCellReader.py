# Dump the cell-reader helper FUN_00454ffc so we know its exact signature.
# @category Creakbox
# @runtime Jython

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor
import os

OUT_DIR = r"C:\GIT\bmd_source\Creakbox\reverse\ghidra_out"
try: os.makedirs(OUT_DIR)
except OSError: pass

TARGETS = [
    ("00454ffc", "cell_reader"),
    ("0045506c", "cell_writer"),
    ("00452edc", "set_row_count"),
    ("00407cac", "int_to_str"),
    ("00407cdc", "str_to_int"),
    ("00408860", "str_to_float"),
    ("00408890", "str_to_int_alt"),
    ("0045dcd0", "get_row_count"),
    ("00403f00", "file_write"),
    ("0040585b", "file_writeln"),
    ("004226a0", "file_reset"),
    ("00405468", "file_close"),
]

program = currentProgram
af = program.getAddressFactory().getDefaultAddressSpace()
fm = program.getFunctionManager()
decomp = DecompInterface()
decomp.openProgram(program)

for addr_str, tag in TARGETS:
    addr = af.getAddress(long("0x" + addr_str, 16))
    func = fm.getFunctionContaining(addr)
    if func is None:
        print("[x] %s (%s): no function" % (tag, addr_str))
        continue
    path = os.path.join(OUT_DIR, "helper_%s_%s.c" % (addr_str, tag))
    result = decomp.decompileFunction(func, 180, ConsoleTaskMonitor())
    with open(path, "w") as fp:
        fp.write("// %s  addr=%s  func=%s\n\n" % (tag, addr_str, func.getName()))
        if result.decompileCompleted():
            fp.write(result.getDecompiledFunction().getC())
        else:
            fp.write("// fail: " + str(result.getErrorMessage()))
    print("[*] %s -> %s" % (tag, path))
