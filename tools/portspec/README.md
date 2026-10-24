# Port specification proofs

Executable proofs behind docs/port/ASM_SPEC.md (asm/, worker asmspec 2026-10-24) and docs/port/TILE_CODE.md
(tiles/, worker tilecode 2026-10-24). Each script runs original DOS code from the decoded oracle image in Unicorn and
compares it with an independent Python model of the documented behaviour (EQUIVALENT_TESTED, not STRICT).
Run from the repository root, e.g. `python -B tools/portspec/asm/prove.py --count 256` and
`python -B tools/portspec/tiles/prove.py`; generated results stay next to the scripts (git-ignored). The specs'
reproduction notes name the original worker paths build/workers/asmspec and build/workers/tilecode.
