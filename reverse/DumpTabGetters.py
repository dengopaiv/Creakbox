# @category Creakbox
# @runtime Jython

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor
import os

OUT_DIR = r"C:\GIT\bmd_source\Creakbox\reverse\ghidra_out"
try: os.makedirs(OUT_DIR)
except OSError: pass

TABS = [
    ("getVolume", "0045d260"),
    ("getTune",   "0045d3d4"),
    ("getCutoff", "0045d52c"),
    ("getRezo",   "0045d710"),
    ("getEnvMod", "0045d868"),
    ("getDecay",  "0045d9c4"),
    ("getAccent", "0045db64"),
]

program = currentProgram
af = program.getAddressFactory().getDefaultAddressSpace()
fm = program.getFunctionManager()
decomp = DecompInterface()
decomp.openProgram(program)

for name, addr_str in TABS:
    addr = af.getAddress(long("0x" + addr_str, 16))
    func = fm.getFunctionContaining(addr)
    if func is None: print("no func for " + name); continue
    result = decomp.decompileFunction(func, 240, ConsoleTaskMonitor())
    path = os.path.join(OUT_DIR, "tab_%s.c" % name)
    with open(path, "w") as fp:
        fp.write("// " + name + " @ " + addr_str + "\n\n")
        if result.decompileCompleted():
            fp.write(result.getDecompiledFunction().getC())
        else:
            fp.write("// fail: " + str(result.getErrorMessage()))
    print("wrote " + path)
