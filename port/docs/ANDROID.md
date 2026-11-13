# Android build (arm64-v8a, x86_64)

The Android app is the same native port: `port/CMakeLists.txt` (its `ANDROID` branch) builds it as `libmain.so` for
SDL's Android activity, next to `libSDL3.so` built from the pinned SDL 3.4.16 release tarball. `port/android/` is a
small Gradle project around it. The official Android release (Rayman Classic 1.0.1) shipped armeabi-v7a/x86 only and
no longer installs on current devices; its touch controls are reproduced from its own scripts
([TOUCH_CONTROLS.md](TOUCH_CONTROLS.md)).

## Build

Requirements: Android SDK with platform 36, NDK 27.0.12077973 and CMake 3.22.1 (SDK manager versions); JDK 17 or 21
for Gradle 8.12; Python 3 on the build machine; the SDL 3.4.16 source tarball (default
`C:/tools/download/sdl3/SDL3-3.4.16.tar.gz`, hash-checked; `-PsdlTarball=FILE` elsewhere). The build reads no game
file (`-PrayData=DIR` only names the game folder for the desktop tests CMake configures).

```bash
cd port/android && JAVA_HOME="C:/Program Files/Java/jdk-17" ./gradlew.bat assembleDebug -Ppython=PATH_TO_python.exe
```

`-Pabis=arm64-v8a` (phones) or `-Pabis=x86_64` (emulator) builds one ABI; the default is both. The APK is
`port/android/app/build/outputs/apk/debug/app-debug.apk`; `assembleRelease` gives a release build signed with the
debug key (personal builds). `local.properties` (git-ignored) holds `sdk.dir`.

## Game data

The APK contains no game files. On the first start (`SetupActivity.java`, user decision 2026-11-13) the app asks the
player to pick their game folder with Android's folder picker: GOG's "Rayman Forever" folder, or a Rayman v1.21 game
folder (the one with RAY.LNG). It copies what the game needs into the app's external files folder
`Android/data/org.raymanrecon.port/files/`, laid out like GOG's install, with a progress bar, then starts the game:

| Picked | Copied to `files/` |
|---|---|
| GOG's Rayman Forever | `Rayman/` (the game), `game.ins` and `game.gog` (GOG's CD), `Music/` (its Ogg tracks), `cloud_saves/`, and any `*.cue`/`*.bin` next to them (about 190 MB) |
| a game folder with RAY.LNG | all of it into `Rayman/` (its own CD image or movies included) |

Programs and shortcuts (`.exe`, `.dll`, `.lnk`) are skipped: the app reads only data files. The copy goes to
`files/.incoming/` first and is moved into place when complete, so an interrupted setup never starts a half-copied game
(the next start asks again). Later starts go straight to the game (`RaymanActivity`). To pick another folder, clear
the app's storage in Android's settings.

The native side then works as on Windows (port/README.md): `files/Rayman/` is the game folder (`rayman_sdl3.json` is
created there), `cd: "auto"` finds GOG's `game.ins` one level up (or an original CD image, which it prefers), saves and
RAYMAN.CFG go to `files/cloud_saves/` when it exists (else the app's private storage, `SDL_GetPrefPath`), and
`files/rayman_log.txt` receives what the desktop build prints. Game files copied over USB or `adb push` straight into
`files/` (RAY.LNG there) still work, without the setup:

```bash
adb push assets/GOG/. /sdcard/Android/data/org.raymanrecon.port/files/
```

Native code is always built optimised (`RelWithDebInfo`, `-O2 -g`), the debug APK included; `-PnativeBuildType=Debug`
builds it `-O0` for debugging only. Measured on a Galaxy S24 Ultra (2026-11-12, Pink Plant Woods, widescreen 100
columns per side, `pace_trace.txt`): `-O0` 36.8 fps with the native renderer at 10.6 ms per frame (median) and the
machine clock at 0.694 x real time, so gameplay and CD music ran at about 69 % speed; `-O2` 60 fps, native renderer
about 4 ms per frame, machine clock 1.003 x (display lock at 120 Hz). The native renderer draws pixel by pixel
(`render/native.c put`), so it is the cost that unoptimised builds and slower phones feel first.

Testing: an optional `args.txt` in the app's `files/` folder holds command-line options of the desktop build,
whitespace-separated (e.g. `--demo 3`), read by `RaymanActivity.getArguments`. Delete it for normal play.

Diagnostics: an empty file `pace_trace.on` in the app's `files/` folder makes the app write `pace_trace.txt` there (the
`RAYMAN_PACE_TRACE` line per frame, including `native_us`, the native renderer's time).

## Platform behaviour

- Landscape (`sensorLandscape`), always fullscreen. Widescreen is on by default and as wide as the screen: the number
  of extra columns per side comes from the display's aspect ratio and the pixel shape (`port_host_fit_pad`, up to
  160 per side). Square pixels (`"square_pixels": true`, the default on every platform, user decision 2026-11-12)
  show the 320x200 picture as 16:10, so a 3120x1440 phone gets 57 columns per side (100 with the original 4:3 pixel
  shape). Menus, movies and other screen-space scenes drawn from 320-column pictures have black sides
  (RENDERER.md section 9, the sides). `"widescreen": "off"` in `rayman_sdl3.json`, or the Display screen (Options,
  Graphics Details, Video mode), gives the original 320-column view. A desktop build with `"fit"` fits its window or,
  fullscreen, its display. The game always renders 320x200: its "X mode" is the unchained (planar) VGA
  layout at 200 lines (`InitModeX` 1:046b8 sets no vertical timing), and the port runs the normal mode.
- Back key: Esc (pause in a level, back in the DOS menus), like the release's back-button listener.
- Input: the virtual pad (`platform/pad.c`): a gamepad and the touch overlay (`platform/touch.c`) set the same pad
  buttons, which become the game's keys. See TOUCH_CONTROLS.md "Port implementation".
- 64-bit `long`: Android (and Linux) are LP64. The only historical unit that declared Watcom `long` types for 32-bit
  values, `savecodec.c`, has a derivative using `int` (derive/provenance.json). All six original demos are identical
  to the reviewed golden summaries on an LP64 build (`build/port-linux`, WSL Ubuntu, gcc 15, 2026-11-12), with every
  other ctest gate passing.

## Status

- PROVEN on the x86_64 emulator (API 36, 2340x1080 and 3120x1440), touch only: logo, intro, main menu, save creation
  and selection, world map (cross + GO), level (floating stick, jump), Pause opening the DOS in-level options menu,
  Back returning to the level; the firefly demo (demo 3) at pad 100 with clean margins.
- PROVEN on a phone (Galaxy S24 Ultra, arm64-v8a, Android 17, 3120x1440, 2026-11-12): played by touch by the user;
  60 fps with CD music at full speed in the -O2 build (measurements above).
- Not yet done: the release's button artwork (the overlay draws plain discs and labels; the art can be read from the
  player's OBB, see TOUCH_CONTROLS.md open points); the touch overlay is shown during the attract-mode demos (it
  follows the game's context: menus or gameplay); behaviour when the app is backgrounded.
- OPEN: the user saw corrupted screen edges in the firefly demo on the phone (an earlier build); not reproduced on the
  desktop or the emulator at the phone's resolution.
