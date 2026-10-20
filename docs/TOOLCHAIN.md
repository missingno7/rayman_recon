# Tools under C:\tools used by this project

Pins (hashes, key files, profiles): `config/toolchain.json`. All installs are treated as immutable.

| tool | location | provenance | status |
|---|---|---|---|
| Watcom C/C++ 10.6 | `C:\tools\watcom-10.6` | archive.org `Watcom_C_10.6` / `Watcom_C++_10.6.iso` (CD vol. WATCOM_C106, 1996-03-13); SHA-1 `211f4e7a…` equals archive.org metadata; SHA-256 `18dd98ae…aecf2`; 7-Zip extraction, no setup run | **in use** (game profile); native via `BINNT\WCC386.EXE` stub + `BINW\WCC386.EXE` payload |
| Watcom C/C++ 10.5 (ISO tree) | `C:\tools\watcom-10.5` | archive.org `Watcom_C_10.5` / `Watcom_C++_10.5.iso` (WATCOM_C105, 1995-07-14); SHA-1 `0722e749…` matches metadata; SHA-256 `e7d8275b…808b9b`; verbatim 7-Zip extraction | libraries OK; plain-tree compiler truncated (65,536-byte `BINW\WCC386.EXE`) |
| Watcom C/C++ 10.5 (derived) | `C:\tools\watcom-10.5-pck` | ISO `BINNT/BINW/H/LIB386` + `BINW\WCC386.EXE` unpacked from the same ISO's `DISKIMGS` (PCK00017, CRC-checked, 567,558 B, SHA-256 `1b97a68f…`; the truncated copy is its exact prefix) with `tools/wpack.py`; 7,078 of 7,233 packed members byte-identical to the plain tree; `PROVENANCE.txt` inside | **runs natively** (banner Version 10.5); control |
| Watcom 10.0 GA, 10.0a, 9.5b | `C:\tools\watcom-10.0`, `-10.0a`, `-9.5b` | installed earlier by kegg_recon (see its toolchain.json) | controls (10.0/10.0a run natively) |
| Watcom 10.5 floppy set (PWA scene release) | `C:\tools\download\watcom\watcom-10.5-win95nt.zip` | 1995 warez-group repack | **not used** (policy: no repacked/cracked tools) |
| PMODE/W 1.33 + source | `C:\tools\pmodew-1.33` | official http://www.sid6581.net/pmodew/ (MIT since 2023-07); archives in `C:\tools\download\pmodew` | format reference only |
| Python 3.11 + capstone 5.0.7 | user install | - | all tools |
| MS-DOS Player | `C:\tools\nmlgcdos\msdos.exe` | ReC98 build (pinned by kegg_recon) | preferred DOS host (not needed yet) |
| DOSBox-X | `C:\tools\dosbox-x*` | official releases (see C:\tools\CATALOG.md) | fallback for extended DOS programs (not used yet) |
| Ghidra 11.3.1 / 12.1 | `C:\tools\ghidra_*` | - | available, not used |
| 7-Zip 22.01 | `C:\Program Files\7-Zip` | system | ISO extraction |

Reference code fetched for porting (read-only, `scratch/ref/`, ignored): pmw1-rs (PluMGMK, commit ed92312a) - not
used by the tools; Open Watcom v2 `bld/wpack` (commit 979e202f) - format reference for the 10.5 PCK unpacker.
`tools/omf.py` is copied from kegg_recon (same author).

Not used: PMWUNLIT/PMWUNLIC third-party unpackers named by rayman_disasm (one is a cracked build); our decoder is
derived from the official source instead.
