# Ghidra script - finds the .cb reader/writer functions in CREAKBOX.EXE and
# dumps their decompiled code.
# @category Creakbox
# @runtime Jython

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor
from ghidra.program.model.scalar import Scalar
import os

OUT_DIR = r"C:\GIT\bmd_source\Creakbox\reverse\ghidra_out"
try:
    os.makedirs(OUT_DIR)
except OSError:
    pass

KEYWORDS = [
    "CreakBox",
    ".cb",
    "Cannot open",
]
# Integer immediates that appear only in the .cb reader/writer code path.
# 9999 is the sweep sentinel.
INT_MARKERS = [9999]

program = currentProgram
listing = program.getListing()
fm = program.getFunctionManager()
rm = program.getReferenceManager()
monitor = ConsoleTaskMonitor()

funcs_of_interest = {}  # key -> {"func":..., "hits": set}

def mark(func, hit):
    if func is None: return
    entry = str(func.getEntryPoint())
    info = funcs_of_interest.setdefault(entry, {"func": func, "hits": set()})
    info["hits"].add(hit)

# 1) Data-driven hits (strings).
print("[*] scanning defined data")
di = listing.getDefinedData(True)
while di.hasNext():
    d = di.next()
    v = d.getValue()
    if v is None: continue
    s = str(v)
    for kw in KEYWORDS:
        if kw in s:
            for r in rm.getReferencesTo(d.getAddress()):
                mark(fm.getFunctionContaining(r.getFromAddress()), kw)

# 2) Integer-immediate hits - walk every instruction, check scalar operands.
print("[*] scanning immediate operands for %r" % INT_MARKERS)
insts = listing.getInstructions(True)
n = 0
while insts.hasNext():
    ins = insts.next()
    n += 1
    for op in range(ins.getNumOperands()):
        obj = ins.getDefaultOperandRepresentation(op)
        # Scalar operand objects
        for ref in ins.getOpObjects(op):
            if isinstance(ref, Scalar):
                val = ref.getValue()
                if val in INT_MARKERS:
                    mark(fm.getFunctionContaining(ins.getAddress()), "immediate_%d" % val)
print("[*] scanned %d instructions" % n)

print("[*] functions of interest: %d" % len(funcs_of_interest))
for key in sorted(funcs_of_interest.keys()):
    info = funcs_of_interest[key]
    print("    %s  name=%s  hits=%r" % (key, info["func"].getName(), sorted(info["hits"])))

# 3) Decompile each.
decomp = DecompInterface()
decomp.openProgram(program)
for key in sorted(funcs_of_interest.keys()):
    info = funcs_of_interest[key]
    func = info["func"]
    hits = sorted(info["hits"])
    name = func.getName()
    safe = "".join(c if c.isalnum() or c in "._-" else "_" for c in name)
    path = os.path.join(OUT_DIR, "%s_%s.c" % (key, safe))
    print("[*] decompile -> %s" % path)
    result = decomp.decompileFunction(func, 120, monitor)
    with open(path, "w") as f:
        f.write("// %s @ %s\n" % (name, key))
        f.write("// hits: %r\n\n" % hits)
        if result.decompileCompleted():
            f.write(result.getDecompiledFunction().getC())
        else:
            f.write("// DECOMPILE FAILED: " + str(result.getErrorMessage()))
print("[*] done. output in %s" % OUT_DIR)
