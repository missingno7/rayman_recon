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
| HMI effects/speech | 6,691 checks: API comparisons against current src/hmi bodies; 216 effects, nine speech chunks, 23 checksums, 13 loops; Q16 completion, deterministic busy wait with keyboard interrupt delivery; both builds |
| Virtual CD drive | 552 checks per build (`tests/cd_tests.c`, `cd_audio_tests.c`, missing-disc path) |
| Compat helpers, record decoder, assets, PIT/retrace model | `tests/unit_tests.c` |
| Source connection | derivatives apply to reviewed bases; PcMain mirror pinned; generated prototypes current; every 64-bit-sensitive compiler site has a verdict |

## Not yet verified

- Exact Sound Blaster mixing arithmetic / hardware clock quantization (the HMI API, source PCM, logical timing and native output are implemented).
- Movies (intro/conclusion FLC), Mode X, joystick/gamepad.
- Frame-synchronized comparison of gameplay against the DOS program over a whole demo (needs a synchronized
  input/RNG replay in DOSBox-X; the per-screen comparison above is exact where the game state coincides).

## HMI-enabled demo regression (2026-10-25)

All six demos keep their existing golden summaries, including exact frame timestamps; no golden update is
proposed. CarteSonAutorisee is now 1 on each run. x86_64 and i686 have byte-identical full frame logs and
full sound traces (bank/program/voice/start/stop/done). StartSample counts by demo 0..5 are
110, 9, 37, 73, 87, 23 (StopSample calls: 35, 38, 50, 37, 32, 33). Sound now updates the game's own sound
bookkeeping; these paths make no RNG draws and do not alter the recorded demo's visible gameplay or
clock waits. The goldens cover rendered images and time, not a hash of every game global.

The isolated validation used MinGW Makefiles because the supplied Ninja runner hung before launching any
compiler in the restricted session. Both architectures used the same working MinGW Python 3.10 test runner;
MinGW Python 3.14's temporary-directory ACL behavior prevented child access in this session. These runner
choices do not change source, flags or tests.

Real-time check: the visible original menus ran for 47 seconds (about nine seconds after HMI initialization),
including two scripted navigation cues. The trace confirms a shared physical SDL device at 48000 Hz, two
channels, format 0x8120 (float32), with HMI data queued; the standalone shared CD/HMI test reports a 480-frame
device buffer. The bounded test stopped its own process after the deadline; no crash occurred before that.
The SDL dummy stream gate also checks simultaneous HMI/CD output and that deterministic mode opens no stream.
