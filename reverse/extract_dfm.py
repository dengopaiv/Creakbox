"""
Extract and decode Delphi Form (DFM) resources embedded in CREAKBOX.EXE.

Scans the binary for TPF0 signatures (the marker Delphi uses for compiled form
resources since Delphi 3), parses each DFM structure, and emits both a raw
binary dump and a textual reconstruction showing every control, property, and
event handler.

Output:
  reverse/forms/<ObjectName>.dfm.bin   raw binary form resource
  reverse/forms/<ObjectName>.dfm       textual Delphi form syntax
"""

from __future__ import annotations
import os
import struct
from pathlib import Path

EXE = Path(r"C:\GIT\bmd_source\Creakbox\CREAKBOX.EXE")
OUT = Path(r"C:\GIT\bmd_source\Creakbox\reverse\forms")

# TValueType tags — from Delphi's Classes.pas
VA_NULL = 0
VA_LIST = 1
VA_INT8 = 2
VA_INT16 = 3
VA_INT32 = 4
VA_EXTENDED = 5
VA_STRING = 6
VA_IDENT = 7
VA_FALSE = 8
VA_TRUE = 9
VA_BINARY = 10
VA_SET = 11
VA_LSTRING = 12
VA_NIL = 13
VA_COLLECTION = 14
VA_SINGLE = 15
VA_CURRENCY = 16
VA_DATE = 17
VA_WSTRING = 18
VA_INT64 = 19
VA_UTF8STRING = 20
VA_DOUBLE = 21


class DfmReader:
    def __init__(self, data: bytes, offset: int = 0):
        self.data = data
        self.pos = offset
        self.start = offset

    def peek(self, n: int = 1) -> bytes:
        return self.data[self.pos:self.pos + n]

    def read(self, n: int) -> bytes:
        b = self.data[self.pos:self.pos + n]
        self.pos += n
        return b

    def read_u8(self) -> int:
        return self.read(1)[0]

    def read_u16(self) -> int:
        return struct.unpack("<H", self.read(2))[0]

    def read_i8(self) -> int:
        return struct.unpack("<b", self.read(1))[0]

    def read_i16(self) -> int:
        return struct.unpack("<h", self.read(2))[0]

    def read_i32(self) -> int:
        return struct.unpack("<i", self.read(4))[0]

    def read_i64(self) -> int:
        return struct.unpack("<q", self.read(8))[0]

    def read_single(self) -> float:
        return struct.unpack("<f", self.read(4))[0]

    def read_double(self) -> float:
        return struct.unpack("<d", self.read(8))[0]

    def read_extended(self) -> float:
        # 80-bit extended float. Approximate to double.
        b = self.read(10)
        mantissa = struct.unpack("<Q", b[:8])[0]
        sign_exp = struct.unpack("<H", b[8:])[0]
        sign = sign_exp >> 15
        exp = sign_exp & 0x7FFF
        if exp == 0 and mantissa == 0:
            return -0.0 if sign else 0.0
        if exp == 0x7FFF:
            return float("nan")
        f = mantissa / (1 << 63)
        f *= 2.0 ** (exp - 16383)
        return -f if sign else f

    def read_pstring(self) -> str:
        # Pascal-style ShortString: 1-byte length + bytes (ASCII / Windows-1252)
        n = self.read_u8()
        return self.read(n).decode("windows-1252", errors="replace")

    def read_lstring(self) -> str:
        n = self.read_i32()
        return self.read(n).decode("windows-1252", errors="replace")

    def read_wstring(self) -> str:
        n = self.read_i32()
        return self.read(n * 2).decode("utf-16-le", errors="replace")

    def read_utf8string(self) -> str:
        n = self.read_i32()
        return self.read(n).decode("utf-8", errors="replace")

    def read_value(self):
        tag = self.read_u8()
        if tag == VA_NULL:
            return ("null", None)
        if tag == VA_LIST:
            items = []
            while self.peek() != b"\x00":
                items.append(self.read_value())
            self.read(1)  # consume terminator
            return ("list", items)
        if tag == VA_INT8:
            return ("int", self.read_i8())
        if tag == VA_INT16:
            return ("int", self.read_i16())
        if tag == VA_INT32:
            return ("int", self.read_i32())
        if tag == VA_EXTENDED:
            return ("float", self.read_extended())
        if tag == VA_STRING:
            return ("string", self.read_pstring())
        if tag == VA_IDENT:
            return ("ident", self.read_pstring())
        if tag == VA_FALSE:
            return ("bool", False)
        if tag == VA_TRUE:
            return ("bool", True)
        if tag == VA_BINARY:
            n = self.read_i32()
            return ("binary", self.read(n))
        if tag == VA_SET:
            idents = []
            while True:
                s = self.read_pstring()
                if not s:
                    break
                idents.append(s)
            return ("set", idents)
        if tag == VA_LSTRING:
            return ("string", self.read_lstring())
        if tag == VA_NIL:
            return ("nil", None)
        if tag == VA_COLLECTION:
            items = []
            # Each item: flags byte, then optional fields, then properties
            while self.peek() != b"\x00":
                flags = self.read_u8()
                item_props = []
                idx = None
                if flags & 1:
                    # classname override
                    _ = self.read_pstring()
                if flags & 2:
                    # item index
                    idx_vt = self.read_u8()
                    if idx_vt == VA_INT8:
                        idx = self.read_i8()
                    elif idx_vt == VA_INT16:
                        idx = self.read_i16()
                    elif idx_vt == VA_INT32:
                        idx = self.read_i32()
                while self.peek() != b"\x00":
                    pname = self.read_pstring()
                    pval = self.read_value()
                    item_props.append((pname, pval))
                self.read(1)
                items.append((idx, item_props))
            self.read(1)
            return ("collection", items)
        if tag == VA_SINGLE:
            return ("float", self.read_single())
        if tag == VA_CURRENCY:
            return ("currency", self.read_i64() / 10000.0)
        if tag == VA_DATE:
            return ("date", self.read_double())
        if tag == VA_WSTRING:
            return ("string", self.read_wstring())
        if tag == VA_INT64:
            return ("int", self.read_i64())
        if tag == VA_UTF8STRING:
            return ("string", self.read_utf8string())
        if tag == VA_DOUBLE:
            return ("float", self.read_double())
        raise ValueError(f"Unknown DFM tag 0x{tag:02X} at offset 0x{self.pos-1:X}")

    def read_object(self):
        # Object header: flags byte (only meaningful in newer formats), but in
        # TPF0 the object starts directly with ClassName length byte.
        # Actually: Delphi 6+ prefixes with a flag byte; Delphi 3-5 does not.
        # We probe: if byte is a plausible ShortString length (< 0x80 and
        # followed by ASCII), treat it as ClassName. Otherwise treat as flag.
        save = self.pos
        flag_or_len = self.read_u8()
        if flag_or_len & 0xF0 == 0xF0:
            # ffInherited=0x10, ffChildPos=0x20, ffInline=0x40 — the 0xF0 nibble
            # marks a flag byte. Consume, then real length follows.
            classname = self.read_pstring()
            objname = self.read_pstring()
            if flag_or_len & 0x20:
                # child position (int32)
                self.read_i32()
        else:
            # Plain ClassName length
            classname = self.read(flag_or_len).decode("windows-1252", errors="replace")
            objname = self.read_pstring()

        properties = []
        while self.peek() != b"\x00":
            pname = self.read_pstring()
            pval = self.read_value()
            properties.append((pname, pval))
        self.read(1)  # end of properties

        children = []
        while self.peek() != b"\x00":
            children.append(self.read_object())
        self.read(1)  # end of children

        return {"class": classname, "name": objname, "props": properties, "children": children}


def format_value(vt, v, indent: int) -> str:
    pad = "  " * indent
    if vt == "string":
        return f"'{v}'"
    if vt == "int" or vt == "float":
        return repr(v)
    if vt == "bool":
        return "True" if v else "False"
    if vt == "ident":
        return v
    if vt == "nil":
        return "nil"
    if vt == "null":
        return ""
    if vt == "set":
        return "[" + ", ".join(v) + "]"
    if vt == "list":
        inner = ", ".join(format_value(x, y, indent) for x, y in v)
        return "(" + inner + ")"
    if vt == "binary":
        return f"<{len(v)} bytes binary>"
    if vt == "collection":
        out = ["<"]
        for idx, props in v:
            out.append(pad + "  item")
            for pname, (pvt, pv) in props:
                out.append(f"{pad}    {pname} = {format_value(pvt, pv, indent+2)}")
            out.append(pad + "  end")
        out.append(pad + ">")
        return "\n".join(out)
    if vt == "currency":
        return f"{v}c"
    if vt == "date":
        return f"{v}d"
    return repr(v)


def emit_object(obj, indent: int = 0) -> list[str]:
    pad = "  " * indent
    head = f"{pad}object {obj['name']}: {obj['class']}"
    if indent == 0:
        head = f"object {obj['name']}: {obj['class']}"
    lines = [head]
    for pname, (vt, v) in obj["props"]:
        val = format_value(vt, v, indent + 1)
        lines.append(f"{pad}  {pname} = {val}")
    for child in obj["children"]:
        lines.extend(emit_object(child, indent + 1))
    lines.append(f"{pad}end")
    return lines


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    data = EXE.read_bytes()
    print(f"Loaded {EXE} ({len(data)} bytes)")
    # Scan for TPF0 signatures
    offsets = []
    i = 0
    while True:
        j = data.find(b"TPF0", i)
        if j < 0:
            break
        offsets.append(j)
        i = j + 4
    print(f"Found {len(offsets)} TPF0 signature(s)")

    for idx, off in enumerate(offsets):
        print(f"\n=== DFM #{idx} at offset 0x{off:X} ===")
        r = DfmReader(data, off + 4)
        try:
            obj = r.read_object()
        except Exception as e:
            print(f"  PARSE FAILED: {e}")
            # Still dump a blob heuristically (up to next TPF0 or 64K)
            end = offsets[idx+1] if idx+1 < len(offsets) else min(off + 65536, len(data))
            blob = data[off:end]
            (OUT / f"unknown_{idx:02d}.dfm.bin").write_bytes(blob)
            continue
        size = r.pos - off
        print(f"  Class: {obj['class']}, Name: {obj['name']}, Size: {size} bytes")
        print(f"  Top-level properties: {len(obj['props'])}, children: {len(obj['children'])}")
        stem = obj["name"] or f"form_{idx:02d}"
        blob = data[off:r.pos]
        (OUT / f"{stem}.dfm.bin").write_bytes(blob)
        text_lines = emit_object(obj)
        (OUT / f"{stem}.dfm").write_text("\n".join(text_lines), encoding="utf-8")
        print(f"  -> wrote {stem}.dfm ({len(text_lines)} lines) + {stem}.dfm.bin")


if __name__ == "__main__":
    main()
