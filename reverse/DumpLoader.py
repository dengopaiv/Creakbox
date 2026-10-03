# @category Creakbox
# @runtime Jython

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

program = currentProgram
af = program.getAddressFactory().getDefaultAddressSpace()
fm = program.getFunctionManager()
listing = program.getListing()
decomp = DecompInterface()
decomp.openProgram(program)

# Loadmodule1Click was at 0x00462ADC per earlier extraction. Maybe it
# wasn't analyzed as a function - try to create one.
addr = af.getAddress(long("0x00462adc", 16))
func = fm.getFunctionContaining(addr)
if func is None:
    print("No function at 0x00462adc. Creating one.")
    from ghidra.app.cmd.function import CreateFunctionCmd
    cmd = CreateFunctionCmd(addr)
    ok = cmd.applyTo(program, ConsoleTaskMonitor())
    print("CreateFunctionCmd: " + str(ok))
    func = fm.getFunctionContaining(addr)

if func is None:
    print("Still no function. Giving up.")
else:
    result = decomp.decompileFunction(func, 240, ConsoleTaskMonitor())
    path = r"C:\GIT\bmd_source\Creakbox\reverse\ghidra_out\method_Loadmodule1Click.c"
    with open(path, "w") as fp:
        fp.write("// Loadmodule1Click at " + str(func.getEntryPoint()) + "\n\n")
        if result.decompileCompleted():
            fp.write(result.getDecompiledFunction().getC())
        else:
            fp.write("// decompile failed: " + str(result.getErrorMessage()))
    print("Wrote " + path)
