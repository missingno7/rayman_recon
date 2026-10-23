# Rayman (1995, MS-DOS) - matching decompilation

Goal: original DOS binaries -> verified matching C reconstruction -> source-complete game -> native SDL3 port ->
optional modernisation. The original executable is the authority; matching comes first.

Primary oracle: GOG `RAYMAN.EXE` **v1.21** (SHA-256 `599b1df2…09eb6`); secondary: CD/installed **v1.12**; PS1
`SLUS-000.05` as a semantic reference. See `docs/ORIGINALS.md`, `docs/VERSIONS.md`.

Status: 1,572 STRICT functions (62.5% of game code bytes), all original hand-written assembly STRICT, the complete
program links from source (not yet byte-identical). Numbers: `python tools/status.py`; details: `docs/STATUS.md`;
priorities: `docs/NEXT_STEPS.md`.

## Layout
| path | content |
|---|---|
| `assets/` | original files (user-supplied, ignored, read-only): `GOG/`, `DOS/`, `DOS_CD/`, `PS1/` |
| `external/` | read-only references: rayman-ps1-decomp, rayverse, rayman_disasm |
| `config/` | `oracles.json` (pinned originals), `toolchain.json` (pinned tools + profile), `symbols.json` (name -> address bindings), `units.json` (accepted units, zero-regression baseline) |
| `src/` | reconstructed game C units; each function's verified status is in `config/units.json` |
| `tools/` | extraction, analysis, compile and verification scripts (Python 3.10+, stdlib + capstone) |
| `tests/` | verifier self-tests (positive + negative controls) |
| `docs/` | findings and workflow |
| `build/`, `scratch/` | generated data, worker dirs, experiments (ignored) |

## Requirements
Python 3.10+ with `capstone` (5.0.x) and `pycparser` (permuter only); historical tools under `C:\tools` (override with `RAYMAN_TOOLS`), pinned in
`config/toolchain.json` and listed in `docs/TOOLCHAIN.md`: Watcom C/C++ 10.6 at `C:\tools\watcom-10.6` (required),
10.5/10.0/10.0a as controls.

## Commands (all verified working)
```
python tools/inventory.py --compare      # hash all assets incl. files inside the CD images; DOS install vs CD
python tools/extract.py --all            # PMW1 decode of both DOS oracles -> build/oracles/<id>/ (self-checks)
python tools/libscan.py C:/tools/watcom-10.6/LIB386/DOS/CLIB3R.LIB            # runtime members in the oracle
python tools/libscan.py --compare C:/tools/watcom-10.5/LIB386/DOS/CLIB3R.LIB C:/tools/watcom-10.6/LIB386/DOS/CLIB3R.LIB
python tools/libscan.py --symbols C:/tools/watcom-10.6/LIB386/DOS/CLIB3R.LIB C:/tools/watcom-10.6/LIB386/MATH387R.LIB
python tools/xref.py                     # DOS <-> Rayverse <-> PS1 table -> build/xref/xref.tsv ; `xref.py NAME` queries
python tools/tumap.py --list             # candidate original object boundaries with their functions
python tools/disasm.py myRand            # annotated disassembly (name, 1:off, ida:addr)
python tools/wcc.py --profile game src/rand.c -fo=build/rand.obj      # pinned, isolated compile
python tools/match.py src/rand.c         # compile + strict verify + first-difference report
python tools/verify.py                   # clean rebuild of all accepted units, zero-regression gate
python -m unittest discover -s tests     # verifier self-tests
python tools/status.py                   # progress derived from config/units.json + last verify report
python tools/promote.py OVERLAY.json [OLD=NEW]      # integrator: merge worker bindings (refuses conflicts/overlaps)
python tools/promote.py --unit NAME src/X.c         # integrator: register a unit with freshly measured statuses
python tools/seed.py draft 14e90 15e60      # seed an untouched object from PS1/Rayverse C into a draft unit (docs/SEEDING.md)
python tools/seed.py try NAME           # seed one function, compile in context, diagnose against the oracle
python tools/census.py && python tools/census.py --brief UNIT   # structural NEAR diagnosis + worker brief (docs/CENSUS.md)
python tools/explain.py UNIT FUNC         # why a NEAR function differs (allocator/scheduler mechanisms)
python tools/permute.py SRC.c FUNC --aggressive --seed 1 --max-compiles 800 --out build/permute/X   # source permuter
python -B tools/manifest.py && python -B tools/link.py all   # natural link of the whole program (build/link)
python tools/wpack.py verify             # one-off: unpack the Watcom 10.5 CD install set (provisioning, docs/TOOLCHAIN.md)
```
`--oracle dos_v112_us` selects the secondary oracle for `libscan`, `disasm`, `tumap`, `match`.
First-time order: `inventory` (optional) -> `extract --all` -> `libscan --symbols` (each oracle, with its libraries:
10.6 for v1.21, 10.5 for v1.12) -> `xref` -> `verify`.

## Rules
`AGENTS.md` (working rules), `docs/MATCHING.md` (workflow, status definitions), `docs/COMPILER.md` (evidence).
