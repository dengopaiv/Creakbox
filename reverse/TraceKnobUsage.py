# @category Creakbox
# @runtime Jython
#
# For each tab's bp0.Value and bp1.Value global, find all instructions
# that reference those addresses anywhere in the program. Then, for each
# reference, find the containing function so we can see where the tab
# values get USED in the DSP (not just where they're LOADED by FUN_0045dd70).

from ghidra.util.task import ConsoleTaskMonitor

program = currentProgram
af = program.getAddressFactory().getDefaultAddressSpace()
rm = program.getReferenceManager()
fm = program.getFunctionManager()
listing = program.getListing()

# From FUN_0045dd70, the 7 tabs' bp0.Value and bp1.Value globals:
TAB_VALUES = [
    ("Volume", 0x0046889a, 0x004688ae),
    ("Tune",   0x004688c2, 0x004688d6),
    ("Cutoff", 0x004688ea, 0x004688fe),
    ("Rezo",   0x00468912, 0x00468926),
    ("EnvMod", 0x0046893a, 0x0046894e),
    ("Decay",  0x00468962, 0x00468976),
    ("Accent", 0x0046898a, 0x0046899e),
]

for name, v0, v1 in TAB_VALUES:
    print("=== %s ===" % name)
    for label, addr_val in [("bp0.Value", v0), ("bp1.Value", v1)]:
        addr = af.getAddress(long(hex(addr_val), 16))
        refs = rm.getReferencesTo(addr)
        print("  %s @ %s" % (label, addr))
        funcs_seen = set()
        for r in refs:
            from_addr = r.getFromAddress()
            func = fm.getFunctionContaining(from_addr)
            fname = func.getName() if func else "<none>"
            key = (fname, str(func.getEntryPoint()) if func else "")
            if key in funcs_seen: continue
            funcs_seen.add(key)
            print("    used in %s (@%s) e.g. ref from %s" %
                  (fname, func.getEntryPoint() if func else "?", from_addr))
