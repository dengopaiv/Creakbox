# @category Creakbox
# @runtime Jython

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

program = currentProgram
af = program.getAddressFactory().getDefaultAddressSpace()
fm = program.getFunctionManager()
decomp = DecompInterface()
decomp.openProgram(program)

TARGETS = ["00454274", "00454fa8", "00454f44", "00454ef8"]
for addr_str in TARGETS:
    addr = af.getAddress(long("0x" + addr_str, 16))
    func = fm.getFunctionContaining(addr)
    if func is None:
        print("NO FUNC: " + addr_str); continue
    result = decomp.decompileFunction(func, 180, ConsoleTaskMonitor())
    print("=== " + addr_str + " ===")
    if result.decompileCompleted():
        print(result.getDecompiledFunction().getC())
    else:
        print("FAIL: " + str(result.getErrorMessage()))
    print()
