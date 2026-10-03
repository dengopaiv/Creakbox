"""
Systematically analyze .cb preset files to reverse the text schema.

Strategy:
  1. Parse each .cb as a list of text lines (CRLF-delimited).
  2. Tabulate total line count across all files.
  3. Locate the 9999 sentinels and measure the distance between them.
  4. Correlate file-length-in-lines against a model: header(3) + globals(N) + 16 steps * P per-step params + something trailing.
"""

from __future__ import annotations
import os
from pathlib import Path
from collections import Counter

MODULES = Path(r"C:\GIT\bmd_source\Creakbox\modules")


def load(p: Path) -> list[str]:
    return p.read_bytes().decode("windows-1252").split("\r\n")


def main():
    files = sorted([*MODULES.glob("*.cb"), *MODULES.glob("time for acid/*.cb")])
    print(f"Found {len(files)} .cb files\n")

    # Summary stats
    print(f"{'file':45s} {'lines':>6s} {'9999s':>6s} {'first9999':>10s} {'last9999':>9s}")
    print("-" * 80)
    stats = []
    for f in files[:12]:  # sample 12
        lines = load(f)
        if lines and lines[-1] == "":
            lines = lines[:-1]
        nlines = len(lines)
        sentinels = [i for i, ln in enumerate(lines) if ln == "9999"]
        first = sentinels[0] if sentinels else -1
        last = sentinels[-1] if sentinels else -1
        stats.append((f.name, nlines, len(sentinels), first, last, lines))
        print(f"{f.name[:44]:45s} {nlines:>6d} {len(sentinels):>6d} {first:>10d} {last:>9d}")

    # Pick the shortest — 303power.cb should be a minimal pattern
    short = min(stats, key=lambda s: s[1])
    print(f"\n\n=== Full dump of shortest: {short[0]} ===")
    for i, ln in enumerate(short[5]):
        marker = "  <-- 9999" if ln == "9999" else ""
        print(f"  line {i:3d}: {ln!r}{marker}")

    # Spacing analysis: distances between successive 9999s
    print(f"\n\n=== 9999-sentinel spacing across files ===")
    for name, nlines, nsent, f9, l9, lines in stats:
        sent = [i for i, ln in enumerate(lines) if ln == "9999"]
        if len(sent) < 2:
            continue
        gaps = [sent[i+1] - sent[i] for i in range(len(sent)-1)]
        head_gap = sent[0]
        print(f"  {name:35s} head={head_gap}  gaps={Counter(gaps).most_common()}  tail={nlines-sent[-1]-1}")


if __name__ == "__main__":
    main()
