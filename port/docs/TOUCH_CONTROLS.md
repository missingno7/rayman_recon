# Touch controls: specification from the Android release

Goal: the port's touch controls (Android ARM64 build) behave exactly like the Android release
`com.ubisoft.raymananniversary` 1.0.1 (armeabi-v7a/x86 only; it no longer runs on current devices).
This file records what that release does, read from the player's own copy. It is a specification written in our own
words, not a copy of the scripts. Evidence levels: PROVEN (read directly from the scripts or the native code),
HYPOTHESIS, OPEN.

## Where the behaviour lives (PROVEN)

- The APK holds the Spark2 engine (`lib/*/libSparkApp.so`, the game code compiled for ARM/x86 plus Lua bindings) and
  engine scripts. The game's own scripts and GUI art are in the expansion file
  `main.12.com.ubisoft.raymananniversary.obb` (a stored ZIP, `Spark2Projects/...`).
- Every `*.spd` member is AES-128-CBC, zero IV, PKCS#7 padding, with a constant key built by
  `SparkUtils::DecryptBuffer` (x86 0x84de7f..0x84deca; `Rijndael_cipherInit` mode 2 = CBC, IV NULL). Member names are
  obfuscated as well (`SparkUtils::DecryptFilename`; not needed so far). Decrypted members are Lua class scripts
  (text) or PNG images. `port/tools/android_obb.py` lists and decrypts members into an ignored directory.
- The Lua layer decides layout, hit tests and contexts; it then calls native bindings with a button id.
  The scripts quoted below by class name live in `Spark2Projects/RaymanGameManager/` (`RayJoystick`,
  `RaymanActionButton`, `RaymanJumpButton`, `PakRaymanHUD`, `PauseButton`, `HUDButtonFader`, `MenuGUI`,
  `RaymanGameManager`) and `Spark2Projects/_TouchInput/TouchJoysticks/` (`PakTouchJoystick`).

## Screen coordinates (PROVEN unless marked)

Positions are in a 2D GUI space: x from -1 (left edge) to +1 (right edge), y from -1 (bottom) to +1 (top).
Joystick distances are measured in x units: a vertical offset is divided by the screen aspect ratio
(width/height) before use. Button sizes come from the texture's pixel size times a scale factor (`RealPixelSize`);
the reference resolution that turns texture pixels into GUI units is OPEN (read it from the GUI layer before
fixing button sizes).

## Native button ids

The Lua side calls `setButtonPressed(id)`, `setButtonReleased(id)` (edge) and `setButtonHeld(id)` (each frame while
held). Ids follow the gamepad list of `RaymanGameManager._Buttons` (1-based):

| id | pad | native flag | DOS key written each frame | meaning in a level |
|---:|---|---|---|---|
| 1 | A | `g_iKeyA` | Space (0x39) | jump |
| 2 | B | `g_iKeyB` | Enter (0x1C) | fist |
| 3 | X | `g_iKeyStart` | X (0x2D) | action (plant seed, run, grimace) |
| 4 | Y | - | - | none |
| 5 | Start | menu flags | - | popups only |
| 6 | Back | `g_iKeyC` / menu flags | Esc (0x01) | pause (via `setButtonPressed(6)`) |
| 7..10 | LB RB L3 R3 | - | - | none |
| 11 | Up | `g_iKeyUp` | Up (0x48) | up |
| 12 | Down | `g_iKeyDown` | Down (0x50) | down |
| 13 | Left | `g_iKeyLeft` | Left (0x4B) | left |
| 14 | Right | `g_iKeyRight` | Right (0x4D) | right |

Sources: x86 `setButtonPressed` 0x557460, `setButtonReleased` 0x5575a0, `setButtonHeld` 0x558850 (15-entry switch
tables), `UpdateButtons` 0x55c440 (copies the flags into `Touche_Enfoncee[scancode]`), `resetButtons` 0x558af0
(clears every flag each frame). `setButtonHeld` ignores some ids by native game state (`getGameState`): id 1 not in
state 4 (credits); id 2 not in states 2..5 (main, save, credits, world-map menus); id 5 only in states 8/9 (popups);
id 6 not in state 7 (level). Game states: 0 logo, 1 intro, 2 main menu, 3 save menu, 4 credits, 5 world map,
6 continue, 7 level, 8 yes/no popup, 9 quit popup, 10 end credits, 11 choose difficulty.

Port equivalence (HYPOTHESIS until tested): the DOS code reads jump/fist/action through `key_jump`, `key_fist`,
`key_action` (src/input.c `TestKeyJump` etc.). The Android build writes fixed scancodes; the port should press the
player's configured `key_jump` / `key_fist` / `key_action` instead, which is identical with the Android defaults and
stays correct when the player remaps keys.

## Left side: movement stick (PROVEN)

`RayJoystick` (derived from `PakTouchJoystick2D`):

- Active only in HUD contexts `Gameplay`, `Worldmap`, `GameplayRayOnMoskito`, and only while no gamepad is connected.
  Otherwise it ignores touches and is hidden.
- Gameplay (floating stick): a touch that lands with x < -0.2 (and inside the touch area x in [-1.3, 0]) captures the
  stick if no other touch owns it. The stick's centre is the landing point. Moving the same touch gives
  `offset = (touch - centre)` with y divided by the aspect ratio, `value = offset / 0.07`, clamped to length 1
  (direction kept). Release or cancel of that touch returns the value to (0,0). One touch owns the stick at a time.
- World map (fixed D-pad): the centre is fixed at (-0.69, -0.6). A touch counts only if it lands within radius 0.35
  of the centre (aspect-corrected distance), and the value is computed immediately on press from that centre. The
  visual is a cross (`Rayman_CROSS_*`, scaled 1.2x the stick base) whose arm in the 4-way rounded direction is drawn
  opaque, the others at alpha 0.5; the raw value (not the rounded one) drives the game.
- Each frame in the native states world map (5) and level (7) the stick is turned into held directions:
  Down if y < -0.40, else Up if y > 0.50; Right if x > 0.33, else Left if x < -0.33 (both axes may be held). In the
  pause loop the x threshold is 0.25 instead of 0.33.
- Visual: base texture `RYM_PAD_BACK`, knob `RYM_PAD_TOP` (`PakVisualJoystick`, scale coefficient 0.11, knob radius
  0.07 in gameplay, 0 on the world map); colour white alpha 0.80 while touched, 0.10 when idle. The idle stick rests
  at (-0.69, -0.6).

## Right side: action buttons (PROVEN)

Common behaviour (`RaymanActionButton`, a `MenuButton`/`PakGUIButton2D`):
- While pressed it calls `setButtonHeld(ButtonId)` every frame; release just stops that.
- Hit box: the element's half-size times 0.70 on each side unless overridden (multi-touch: each button tracks its
  own touch; the stick and buttons work at the same time).
- Idle colour white alpha 0.7, pressed alpha 1.0, scale factor 0.495.
- Shown and enabled only in its HUD contexts, only when no gamepad is connected, and only when Rayman has the power
  (see each button). Hidden buttons do not react.
- When the game is paused, pressing an action button also validates the pause menu (`validateButtonPressed`).

| button | id | position | hit-box overrides | contexts | shown when | texture |
|---|---:|---|---|---|---|---|
| Jump | 1 | centre (0.86, -0.73) | right 1.0 | Gameplay | power Jump | `RYM_IG_SUPER_HELICO` if super helicopter is available, else `RYM_IG_FLY` if helicopter, else `RYM_IG_JUMP`; label `Rayman_BTN_A` |
| Fist | 2 | above Jump: anchor (0, 1.6), pivot (0, -1) relative to Jump | right 1.0, top 1.0 | Gameplay, GameplayRayOnMoskito | power Punch | `RYM_IG_PUNCH`; label `Rayman_BTN_B` |
| Action | 3 | left of Jump: anchor (-1.6, 0), pivot (1, 0) | - | Gameplay | first available of Plant, Run, Grimace | `RYM_IG_SEED` / `RYM_IG_RUN` / `RYM_IG_BOUH`; label `Rayman_BTN_C` (`Rayman_BTN_X` with a pad) |
| Pause | (6) | (0, 0.8), scale 0.89, idle alpha 0.4 | - | Gameplay, GameplayRayOnMoskito | always | `RYM_IG_PAUSE` |

- Pause: on release the manager enters its Pause state, which plays sound `FIX_12`, switches the HUD context to
  `Pause` and calls `setButtonPressed(6)` (Esc in the native layer). The Android Back key does the same (lowest
  priority back-button listener). Leaving pause restores the previous context.
- In the `Pause` context a Validate button (Jump class, id 1, label `Rayman_BTN_A`) appears at the Jump position.
- Pad connected: `HUDButtonFader` fades HUD elements tagged to hide after 6.0 s (damping 0.40); touch elements are
  disabled. Pad behaviour is outside this document.

## Menus and other screens

The Android release replaced the DOS menus with its own Spark GUI (`MenuGUI`: start, save slots with copy/erase,
difficulty choice, credits, continue, yes/no and quit popups, a world-map GO/OK button at anchor (0.92, -1) that
sends `menuButtonPressed(startWorldMap)` + `validateButtonPressed`). These screens and their native counterparts
(`menuButtonPressed`, `setGamePhase`, `popupStarted`, ...) do not exist in the DOS game the port runs. The port keeps
the DOS menus and drives them by touch with a tap-to-key remote (user decision 2026-11-12; see "Port
implementation"). The brick-breaker minigame (`CasseBriques*`) is Android-only.

## Open points

- OPEN: GUI reference resolution (texture pixels to GUI units) and the atlas that maps texture names
  (`RYM_PAD_BACK`, `RYM_IG_*`, `Rayman_BTN_*`, `Rayman_CROSS_*`) to the decrypted PNGs.
- OPEN: native in-level branch of `UpdateButtons` (`RaymanDansUneMapDuJeu`): it also ORs native `CButton` objects
  (`s_fist`, ...) into Enter/Space/X; check whether any touch path uses them.
- OPEN: which HUD context the native code sets for each scene (`setHUDContext` / `OnSetHUDContext` calls) - needed
  to map the port's scenes to Gameplay / Worldmap / GameplayRayOnMoskito / Pause.

## Port implementation

Like the release, the port has one input path for pad and touch: `platform/pad.c` (virtual pad) and
`platform/touch.c` (overlay), both driven from the thread that polls SDL events (`platform/host.c`).

- **Pad buttons -> keys** (`pad.c key_for`). In levels A/B/X are the player's configured `key_jump` / `key_fist` /
  `key_action` (the release wrote Space / Enter / X, which were its configured keys), Start and Back are Esc, the
  d-pad is `key_up` / `key_down` / `key_left` / `key_right`. On the world map A is Space (validate; the release's GO
  button), Start Enter, Back Esc. On every other screen (the DOS menus, which the release replaced; port decision
  "tap-to-key", user 2026-11-12) A is Enter, B Esc, X Space, Start Enter. The keys go to the game's own keyboard
  interrupt handler as PC scan codes. Every press lasts at least 80 ms so a quick tap is never lost between two of
  the game's once-per-frame key reads.
- **Gamepad**: any SDL gamepad, first one connected; buttons by position (south A, east B, west X, north Y), left
  stick with the release's thresholds (y < -0.40 down, > 0.50 up, |x| > 0.25). The overlay hides once the pad has
  been used (SDL also reports keyboards with d-pad keys as gamepads, e.g. the emulator's).
- **Context** (`render/scene_capture.c port_game_input_context`): world map while `DO_WORLD_MAP` runs; in a level
  (`DO_MAIN_LOOP_PC_NORMAL` with `RaymanDansUneMapDuJeu`) the menu context while `GoMenu` or `OptionGame` (the
  in-level options loop) is set, no HUD while `is_fee` or `Vignet_To_Display`, Ray on the mosquito while `NewMs`,
  else gameplay; no HUD during `START_LEVEL_ANIM`; menu everywhere else.
- **Overlay**: positions, activation areas, stick radius and thresholds, hit-box scales, alphas and visibility rules
  as specified above. Port-only additions: in the menu context the world-map cross, an OK button (A) at the GO
  position and a Back button (B) top left; with the HUD hidden in a level, a finger anywhere holds A (vignettes,
  fairies). Buttons stay pressed while their finger is inside their hit box (HYPOTHESIS for the release's exact
  slide-off behaviour).
- **Differences still open**: plain discs and letters instead of the release's textures; button size
  `0.105 x screen height` instead of the release's texture-based size; the jump icon follows the helicopter powers
  (`RayEvts.helico` / `super_helico`), while the release also checked Rayman's current animation
  (`GetRayCanHelico`); the action icon follows magic seed / run.
- `RAYMAN_TOUCH=1` turns the overlay on in a desktop build (the mouse acts as one finger) for testing;
  `RAYMAN_TOUCH=0` turns it off on Android.
