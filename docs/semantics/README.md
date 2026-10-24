# Semantic registry

One JSON registry per documented unit (`basic.json`: pilot 2026-10-24, src/basic.c + every global and shared field it
touches). Each entity (function, global, type, field, local) records its current name, previous name if renamed, C
declarator, layout (struct, offset, size, signedness, bit width), meaning, unit, value domain, evidence level
(PROVEN / STRONG / HYPOTHESIS / UNKNOWN), evidence (DOS addresses, readers/writers, Android/PS1/Rayverse names) and
port notes (fixed-point scales, sentinels, wraparound, signedness the SDL3 port must keep). UNKNOWN and HYPOTHESIS
entries keep neutral names. Renames are applied only for PROVEN/STRONG meanings and never change compiled output.

- `python -B tools/semantics.py check` - enforced (tests/test_semantics.py): declarations, bindings, the unit's function
  bodies, and the struct layout compiled by the pinned compiler (offsetof/sizeof, bitfield masks).
- `python -B tools/semantics.py check --evidence` - also reports drift of evidence sources (other units' readers and
  writers, assets); the integration script runs it after every integration. Drift means "re-review", not "wrong".
- `python -B tools/semantics.py refresh` - after a reviewed change, re-hash and list what changed.
- `python -B tools/semantics.py generate --out docs/semantics/generated` - port-facing views: `semantics_dos.h` (DOS
  layout facts), `SEMANTICS.md` (entity index), `PORT_CONTRACTS.md` (what the native port must preserve).
