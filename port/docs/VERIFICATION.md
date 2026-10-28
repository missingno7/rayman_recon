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
| Native renderer vs original (`tests/e2e_native.py`, `--verify-render`, fifteen scenarios: six demos, idle menus, key-script new game under three start-up clocks, options menu, in-game menu, pause, credits, intro movie) | whole runs, including menus, name entry, world map, three transition effects, the in-game menu, pause, credits and the intro movie: every frame (11,812 gameplay frames, 1,758 start-up, title and vignette frames per run) complete (every operation drawn natively) and equal to the original draw buffer and display over the full 320x200 image; every presented image equal; frame logs of `--renderer native` runs identical to the reference's; every gameplay pass equal to the original's changes; gameplay-state log per swap identical (docs/RENDERER.md 8) |
| Uninitialized-locals policy (`tests/uninit_scope.py`, measurement) | zero vs 0xFE-pattern locals: all six demos identical (x86_64); zero vs host stack: demo 4 state diverges on i686 only, images identical (DIVERGENCES.md) |
| Native wider view (`tests/e2e_wide.py`, `--widescreen`) | gameplay-state logs identical to the original-view run in all six demos; every dumped frame 428 columns; margins drawn in gameplay frames, black where the firefly darkness or a map narrower than the view requires it |

## Not yet verified

- Exact Sound Blaster mixing arithmetic / hardware clock quantization (the HMI API, source PCM, logical timing and native output are implemented).
- Mode X, joystick/gamepad.
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

## Native original movies (2026-10-26)

PROVEN against docs/port/MOVIE_FORMAT.md and the player's read-only assets/DOS files:

- `port_movie_tests`: 40,599 C checks on each architecture, zero failures. An independent scalar decoder
  compares all 64,000 pixels and 768 palette components after every frame: intro 1,805 to EOF (header 1,804),
  conclusion 243 to EOF (header 242). Chunk totals equal the contract (4/7/15/16/18); synthetic fixtures cover
  11/12/13, ignored chunks, word line-skip and the unusual DOS odd-byte coordinates. Centering is (40,25) for
  both files; other geometries are checked. Compact/ordinary FLI/FLC, read/format/dimension/CPU errors,
  successful short reads/EOF, ordered roots/save priority, PIT0 latch/reload/BIOS time, one-shot vs looping input
  order, alternate-frame playback and all 32 fade writes/synchronizations are checked. Controlled CD tests
  force pauses 0..3, resumes 1..4, exact-end advances 0..4 and the -1 sentinel. The full native conclusion
  entry plays 242 frames with CD track 21 and HMI enabled, then restores the timer registration.
- All CTest gates pass: x86_64 19/19, i686 20/20 (including the extra unmodified effect-reference gate).
  MinGW Makefiles, GCC 12.2 x64 / 16.2 i686, MinGW Python 3.10.11; all temporary output in the worker directory.
- `port_movie_e2e`: original menus, intro, world map, original exit confirmation, main menu (5,000 changed-image
  records). x64 and i686 full logs are byte-identical, including timestamps; SHA-256
  `3427b5b31ff5962b486e2addc3cef1514b6c1394f9fd9600ef97e55535788297`. The final image is the main menu:
  `4999 209338597380 da04105cf7183103`. All 49 paired evidence files (six demos, movie/skip/demo-with-movies,
  CD/HMI traces and summaries) are identical. The six existing golden files are unchanged; demo 0 with both
  data roots also matches its existing golden and never enters the movie player.
- Intro: 1,804 selected decoded frames, 1,777 changed images during the whole player (1,772 during decoding,
  with the monitor deduplicating unchanged pixels/palettes). Player entry/return 56.613688065/178.263971825 s;
  first-to-last decoded frame 120.097239075 s, final fade 1.034879840 s. The original BIOS/PIT unsigned timer
  calculation is retained: 1,794 intervals of 66.859980 ms and nine of 16.714995 ms, not header-speed pacing.
  Device commands are PLAY 84h, STOP/PAUSE 85h and RESUME 88h. There are six STOPs before return-time cleanup,
  one PLAY and one RESUME; cleanup adds two STOPs. In deterministic fast-machine time no threshold is reached
  inside a pause window; rows advance at exact ends 223/468/920/1195, and frame 1 resumes the initial pause.
- Omitting the second data root returns -5 without advancing the clock, and the menus/game continue. The extra
  DOS directory also contains RAYMAN1.SAV, so save selection differs between the two general search paths;
  equal pre-movie menu walks are not claimed. No movie is taken from the virtual CD image.

PROVEN real-time presentation on the final x64 executable (SHA-256
`6e53b5e5534372272ee78d205de1bae9fa2160f80fc9f6673b8d7aaa8ad11be6`): visible 2x window, SDL audio,
no deterministic/headless flags, full 1,804-frame intro and fade without interruption, natural frame-limit exit
at 194.86 wall seconds. Movie entry/return 56.5986094/181.3399526 s; first-to-last frame 123.189573 s;
maximum frame interval 120.4666 ms, no stall or crash. CD pauses occur at frames 221/453/913/1187 (rows 0..3),
resumes at 223/468/920/1195 (rows 1..4), plus initial frame 1 resume. The shared SDL device reports 48 kHz,
stereo float32, 480-frame buffer; CD queue reaches 19,992 bytes, zero on 32 paused frames and never zero on
running frames. Device/queue and command traces are the audio evidence.

Reproduction: `ctest --test-dir BUILD --output-on-failure`. `RAY_MOVIE_TEST_DATA` selects the player's directory
containing the two movie fixtures (default assets/DOS). For full logs set `RAYMAN_E2E_EVIDENCE=DIR`; for visual
samples add `--dump-frames DIR --dump-every 700` to the intro command. Isolated evidence and four reviewed BMPs
(three movie frames and the returned main menu) are in build/workers/port_mov/evidence, outside tracked paths.
This is native contract/corpus and cross-architecture proof; it does not claim a new DOS byte-match result.
