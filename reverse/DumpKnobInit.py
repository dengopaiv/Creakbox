# Decompile the subroutines called at the top of Button4Click, which read
# the 7 grid-tab cells into internal DSP variables.
# @category Creakbox
# @runtime Jython

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor
import os

OUT_DIR = r"C:\GIT\bmd_source\Creakbox\reverse\ghidra_out"
try: os.makedirs(OUT_DIR)
except OSError: pass

# Addresses from the Button4Click decomp
TARGETS = [
    ("0045dd70", "knob_init_common"),
    ("00464a8c", "knob_init_a"),
    ("00464ad4", "knob_init_b"),
    ("004648f4", "knob_init_c"),
    ("00464ae0", "distort_helper"),
    ("00464900", "output_helper"),
    ("00464b18", "lowpass_helper"),
    ("0041beec", "log2_helper"),
    ("0041befc", "pow2_helper"),
    ("0045f524", "getnextnote_helper"),
    ("0045e5d4", "saw_gen"),
    ("0045e608", "square_gen"),
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
    path = os.path.join(OUT_DIR, "sub_%s_%s.c" % (addr_str, tag))
    result = decomp.decompileFunction(func, 180, ConsoleTaskMonitor())
    with open(path, "w") as fp:
        fp.write("// %s  addr=%s  func=%s\n\n" % (tag, addr_str, func.getName()))
        if result.decompileCompleted():
            fp.write(result.getDecompiledFunction().getC())
        else:
            fp.write("// fail: " + str(result.getErrorMessage()))
    print("[*] %s -> %s" % (tag, path))
