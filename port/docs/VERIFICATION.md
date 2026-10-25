# Port verification status

What the port's behaviour is checked against, and with what result (2026-10-25).

## Against the original DOS program

| Check | Reference | Result |
|---|---|---|
| Native normal-mode drawing layer (52 assembly entries) | original routine bytes executed in Unicorn (`tests/asm_diff`) | 14,594 cases, 0 mismatches, 565/565 blocks covered; 3/3 negative controls detected |
| Whole frames: main menu, "CHOOSE A GAME", three level-1 start frames | the rebuilt DOS executable (natural link of `src/`, verified pack round trip) in pinned DOSBox-X, internal 320x200 indexed capture, with the Rayman CD image mounted | menus and two level frames: 0 / 64,000 indices and 0 / 768 DAC components differ; third level frame: 531 pixels at animated butterfly positions (the DOS keystroke timing is not a synchronized replay; STRONG timing hypothesis), landscape, Rayman and HUD identical |
| Presentation | DOSBox-X expands DAC components as `c << 2` | the port uses `(c << 2) \| (c >> 4)` (full-range 8-bit); indices/DAC identical, RGB differs by 0..3 per component |

Procedure and evidence: build/workers/port_ref (REPORT.md, ref_capture.py; not tracked). Original binaries were
never executed: only the rebuilt executable, as required.

## Internal consistency

| Check | Result |
|---|---|
| x86_64 vs i686 (DOS pointer width) on the six original recorded demos (worlds 1-5, CD image) | identical per-frame image logs for all six (2,470-3,579 frames each); golden summaries in `tests/golden` gate both builds |
| Screen effects, derivative vs unmodified units at DOS pointer width | 18,428 effect frames identical (`tests/effect_diff.c`) |
| Determinism | identical frame logs across runs with different heap layouts (`--deterministic`, fixed epoch) |
| Virtual CD drive | 552 checks per build (`tests/cd_tests.c`, `cd_audio_tests.c`, missing-disc path) |
| Compat helpers, record decoder, assets, PIT/retrace model | `tests/unit_tests.c` |
| Source connection | derivatives apply to reviewed bases; PcMain mirror pinned; generated prototypes current; every 64-bit-sensitive compiler site has a verdict |

## Not yet verified

- Audio other than CD-DA (the HMI sound driver is absent: the game runs its no-sound-card path).
- Movies (intro/conclusion FLC), Mode X, joystick/gamepad.
- Frame-synchronized comparison of gameplay against the DOS program over a whole demo (needs a synchronized
  input/RNG replay in DOSBox-X; the per-screen comparison above is exact where the game state coincides).
