"""
Deeper .cb analysis: try to determine steps-per-pattern and pattern count.
"""
from pathlib import Path
MODULES = Path(r"C:\GIT\bmd_source\Creakbox\modules")

def load(p):
    return [x for x in p.read_bytes().decode("windows-1252").split("\r\n")]

# The hypothesis: single-pattern 16-step file is 103 lines.
#  - 3 header (CreakBox, 1.0, BPM)
#  - some patterns
#  - 7 * 9999 sentinels at 7-line intervals (global param tails)
#  - 2 trailing

# Look at section A (pre-first-9999) structure by scanning several sizes
sizes = {}
for f in sorted(MODULES.rglob("*.cb")):
    lines = load(f)
    if lines and lines[-1] == "":
        lines = lines[:-1]
    idx = [i for i, l in enumerate(lines) if l == "9999"]
    if not idx:
        continue
    secA = lines[3:idx[0]]
    key = len(secA)
    sizes.setdefault(key, []).append(f.name)

print("Section-A sizes across all 76 files (count -> example file):")
for k in sorted(sizes):
    files = sizes[k]
    print(f"  A-length = {k:5d}  ({len(files):2d} files)  e.g. {files[0]}")

# All single-pattern files have A=55. Test theory: A grows per pattern.
# (55 - 5) = 50 per pattern if 5 is header overhead. Or some other constant.
print()
for k in sorted(sizes):
    # try y = mx + b fits
    # if 1-pattern is A=55 then m*(P-1)+55 = A => P = (A-55)/m + 1
    # try m = 50: (A-55)/50 + 1
    # try m = 55: (A-55)/55 + 1
    # try m = 48: (A-55)/48 + 1
    candidates = {m: (k-55)/m + 1 for m in (48, 49, 50, 51, 52, 53, 54, 55)}
    whole = {m: v for m, v in candidates.items() if abs(v - round(v)) < 0.001 and v > 0}
    print(f"  A={k}  integer pattern-count candidates: {whole}")

# Separately, look at acidmaker.cb's unusual 32-gap
print("\n\n=== acidmaker.cb detailed sentinels ===")
p = MODULES / "acidmaker.cb"
lines = load(p)
if lines and lines[-1] == "":
    lines = lines[:-1]
idx = [i for i, l in enumerate(lines) if l == "9999"]
print(f"total lines={len(lines)}, sentinels at {idx}")
for i in range(len(idx)-1):
    start, end = idx[i]+1, idx[i+1]
    block = lines[start:end]
    print(f"  between 9999 #{i}-#{i+1}: {end-start} lines: {block[:15]}{'...' if len(block)>15 else ''}")
print(f"  trailer after last 9999: {lines[idx[-1]+1:]}")
