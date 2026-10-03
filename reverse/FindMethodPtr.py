# Ghidra script - finds Delphi method pointers for Button4Click and
# Loadmodule1Click by locating those name strings and checking for
# a 4-byte address right before each Pascal-string length byte, which is
# how Delphi's published method table stores entries.
# @category Creakbox
# @runtime Jython

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor
from ghidra.program.model.address import AddressSet
import os, jarray

OUT_DIR = r"C:\GIT\bmd_source\Creakbox\reverse\ghidra_out"
try: os.makedirs(OUT_DIR)
except OSError: pass

METHOD_NAMES = [
    "Button4Click",
    "Loadmodule1Click",
    "Savemodule1Click",
    "New1Click",
    "FormCreate",
    "eBPMChange",
    "Chaospattern1Click",
    "Chaospatternnicely1Click",
    "eNote1KeyDown",
    "eNote1KeyPress",
]

program = currentProgram
mem = program.getMemory()
fm = program.getFunctionManager()
rm = program.getReferenceManager()
listing = program.getListing()

def scan(pat):
    hits = []
    pbs = [ord(c) for c in pat]
    plen = len(pbs)
    for block in mem.getBlocks():
        if not block.isInitialized(): continue
        size = int(block.getSize())
        if size > 8 * 1024 * 1024: continue
        buf = jarray.zeros(size, "b")
        try: mem.getBytes(block.getStart(), buf)
        except: continue
        for i in range(size - plen + 1):
            if buf[i] & 0xFF != pbs[0]: continue
            ok = True
            for k in range(1, plen):
                if buf[i+k] & 0xFF != pbs[k]:
                    ok = False; break
            if ok:
                # Check that Pascal-string length byte matches name length
                if i >= 1:
                    lb = buf[i-1] & 0xFF
                    if lb == plen:
                        hits.append((block.getStart().add(i), "pascal"))
                        continue
                hits.append((block.getStart().add(i), "raw"))
    return hits

funcs_found = {}  # method_name -> function_entry

for mname in METHOD_NAMES:
    hits = scan(mname)
    for addr, kind in hits:
        if kind != "pascal":
            continue
        # Published methods table: [ addr:4bytes ][ len:1byte ][ name bytes ]
        # The 4-byte address is at (addr - 1 - 4) = addr - 5
        try:
            method_addr_bytes = jarray.zeros(4, "b")
            mem.getBytes(addr.subtract(5), method_addr_bytes)
            method_addr_val = 0
            for k in range(4):
                method_addr_val |= (method_addr_bytes[k] & 0xFF) << (k * 8)
            # sanity-check: address should be in code range
            if method_addr_val >= 0x400000 and method_addr_val <= 0x500000:
                af = program.getAddressFactory().getDefaultAddressSpace().getAddress(method_addr_val)
                f = fm.getFunctionContaining(af)
                name_display = f.getName() if f else "(no func)"
                print("[*] %-25s name@%s  ptr->0x%08X  func=%s" %
                      (mname, addr, method_addr_val, name_display))
                funcs_found[mname] = af
        except Exception as e:
            print("  error for %s at %s: %s" % (mname, addr, e))

# Decompile each found method's function.
decomp = DecompInterface()
decomp.openProgram(program)
for mname, ent in funcs_found.items():
    f = fm.getFunctionContaining(ent)
    if f is None: continue
    path = os.path.join(OUT_DIR, "method_%s.c" % mname)
    result = decomp.decompileFunction(f, 180, ConsoleTaskMonitor())
    with open(path, "w") as fp:
        fp.write("// %s  entry=%s  containing=%s\n\n" % (mname, ent, f.getEntryPoint()))
        if result.decompileCompleted():
            fp.write(result.getDecompiledFunction().getC())
        else:
            fp.write("// decompile failed: " + str(result.getErrorMessage()))
    print("[*] wrote %s" % path)
