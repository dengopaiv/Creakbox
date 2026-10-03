# Ghidra script v2 - scans raw memory for "9999" / "CreakBox" byte patterns and
# finds functions that reference them.
# @category Creakbox
# @runtime Jython

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor
import os
import jarray

OUT_DIR = r"C:\GIT\bmd_source\Creakbox\reverse\ghidra_out"
try:
    os.makedirs(OUT_DIR)
except OSError:
    pass

program = currentProgram
mem = program.getMemory()
fm = program.getFunctionManager()
rm = program.getReferenceManager()
monitor = ConsoleTaskMonitor()

PATTERNS = ["9999", "CreakBox", ".cb"]

def scan_pattern(pat):
    hits = []
    pat_bytes = [ord(c) for c in pat]
    plen = len(pat_bytes)
    for block in mem.getBlocks():
        if not block.isInitialized():
            continue
        size = int(block.getSize())
        if size > 8 * 1024 * 1024:
            continue
        start = block.getStart()
        # Jython bytearray from getBytes returns a jarray of signed bytes.
        java_buf = jarray.zeros(size, "b")
        try:
            mem.getBytes(start, java_buf)
        except Exception:
            continue
        # Convert to unsigned for searching.
        for i in range(size - plen + 1):
            if java_buf[i] & 0xFF != pat_bytes[0]:
                continue
            match = True
            for k in range(1, plen):
                if java_buf[i + k] & 0xFF != pat_bytes[k]:
                    match = False
                    break
            if match:
                hits.append(start.add(i))
    return hits

import jarray

funcs = {}  # key -> (func, set(hits))
def mark(func, tag):
    if func is None: return
    entry = str(func.getEntryPoint())
    info = funcs.setdefault(entry, {"func": func, "hits": set()})
    info["hits"].add(tag)

for pat in PATTERNS:
    addrs = scan_pattern(pat)
    print("[*] %r -> %d byte-level matches" % (pat, len(addrs)))
    for a in addrs:
        for delta in (-4, -1, 0):
            for r in rm.getReferencesTo(a.add(delta)):
                mark(fm.getFunctionContaining(r.getFromAddress()),
                     pat if delta == 0 else (pat + ("_p%d" % delta)))

print("[*] functions of interest: %d" % len(funcs))
for key in sorted(funcs.keys()):
    info = funcs[key]
    print("    %s  name=%s  hits=%r" % (key, info["func"].getName(), sorted(info["hits"])))

decomp = DecompInterface()
decomp.openProgram(program)
for key in sorted(funcs.keys()):
    info = funcs[key]
    func = info["func"]
    name = func.getName()
    safe = "".join(c if c.isalnum() or c in "._-" else "_" for c in name)
    path = os.path.join(OUT_DIR, "%s_%s.c" % (key, safe))
    print("[*] decompile -> %s" % path)
    result = decomp.decompileFunction(func, 180, monitor)
    with open(path, "w") as f:
        f.write("// %s @ %s  hits=%r\n\n" % (name, key, sorted(info["hits"])))
        if result.decompileCompleted():
            f.write(result.getDecompiledFunction().getC())
        else:
            f.write("// DECOMPILE FAILED: " + str(result.getErrorMessage()))
print("[*] done.")
