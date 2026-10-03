# Ghidra script - finds the "Make Wave" render function (Button4Click) in
# CREAKBOX.EXE by looking for functions that reference the hardcoded output
# wav path or invoke disk-write primitives.
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
listing = program.getListing()

# Byte-scan for two strings guaranteed to live in the render path.
PATTERNS = ["c:\\cb303.wav", "Make Wave", "cb303"]

def scan(pat):
    hits = []
    pat_bytes = [ord(c) for c in pat]
    plen = len(pat_bytes)
    for block in mem.getBlocks():
        if not block.isInitialized(): continue
        size = int(block.getSize())
        if size > 8 * 1024 * 1024: continue
        buf = jarray.zeros(size, "b")
        try: mem.getBytes(block.getStart(), buf)
        except: continue
        for i in range(size - plen + 1):
            if buf[i] & 0xFF != pat_bytes[0]: continue
            ok = True
            for k in range(1, plen):
                if buf[i + k] & 0xFF != pat_bytes[k]:
                    ok = False; break
            if ok: hits.append(block.getStart().add(i))
    return hits

funcs = {}
for pat in PATTERNS:
    addrs = scan(pat)
    print("[*] %r -> %d matches" % (pat, len(addrs)))
    for a in addrs:
        for delta in (-4, -1, 0):
            for r in rm.getReferencesTo(a.add(delta)):
                f = fm.getFunctionContaining(r.getFromAddress())
                if f is None: continue
                key = str(f.getEntryPoint())
                funcs.setdefault(key, {"func": f, "hits": set()})["hits"].add(pat)

print("[*] render-candidate functions: %d" % len(funcs))
for key in sorted(funcs.keys()):
    info = funcs[key]
    print("    %s  %s  hits=%r" % (key, info["func"].getName(), sorted(info["hits"])))

decomp = DecompInterface()
decomp.openProgram(program)
for key in sorted(funcs.keys()):
    info = funcs[key]
    f = info["func"]
    path = os.path.join(OUT_DIR, "render_%s_%s.c" % (key, f.getName()))
    result = decomp.decompileFunction(f, 180, ConsoleTaskMonitor())
    with open(path, "w") as fp:
        fp.write("// %s @ %s  hits=%r\n\n" % (f.getName(), key, sorted(info["hits"])))
        if result.decompileCompleted():
            fp.write(result.getDecompiledFunction().getC())
        else:
            fp.write("// DECOMPILE FAILED: " + str(result.getErrorMessage()))
    print("[*] wrote %s" % path)
