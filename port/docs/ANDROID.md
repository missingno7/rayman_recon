# Android build (arm64-v8a, x86_64)

The Android app is the same native port: `port/CMakeLists.txt` (its `ANDROID` branch) builds it as `libmain.so` for
SDL's Android activity, next to `libSDL3.so` built from the pinned SDL 3.4.16 release tarball. `port/android/` is a
small Gradle project around it. The official Android release (Rayman Classic 1.0.1) shipped armeabi-v7a/x86 only and
no longer installs on current devices; its touch controls are reproduced from its own scripts
([TOUCH_CONTROLS.md](TOUCH_CONTROLS.md)).

## Build

Requirements: Android SDK with platform 36, NDK 27.0.12077973 and CMake 3.22.1 (SDK manager versions); JDK 17 or 21
for Gradle 8.12; Python 3 on the build machine; the SDL 3.4.16 source tarball (default
`C:/tools/download/sdl3/SDL3-3.4.16.tar.gz`, hash-checked; `-PsdlTarball=FILE` elsewhere); the player's GOG v1.21
game folder for the two data tables taken from its RAYMAN.EXE at build time (default `assets/GOG`, `-PrayData=DIR`).

```bash
cd port/android && JAVA_HOME="C:/Program Files/Java/jdk-17" ./gradlew.bat assembleDebug -Ppython=PATH_TO_python.exe
```

`-Pabis=arm64-v8a` (phones) or `-Pabis=x86_64` (emulator) builds one ABI; the default is both. The APK is
`port/android/app/build/outputs/apk/debug/app-debug.apk`; `assembleRelease` gives a release build signed with the
debug key (personal builds). `local.properties` (git-ignored) holds `sdk.dir`.

## Game data

The APK contains no game files. The player copies the files of a GOG v1.21 Rayman folder (RAY.LNG, RAYMAN.EXE,
PCMAP, the .DAT files, ...) and the Rayman CD image `RAYMAN.cue` + `RAYMAN.bin` into the app's external files folder
`Android/data/org.raymanrecon.port/files/` (over USB, or `adb push`):

```bash
adb push assets/GOG/. /sdcard/Android/data/org.raymanrecon.port/files/
```

```bash
adb push assets/DOS_CD/RAYMAN.cue assets/DOS_CD/RAYMAN.bin /sdcard/Android/data/org.raymanrecon.port/files/
```

That folder is the game folder of the desktop build: `rayman_sdl3.json` (settings) is created there on the first run,
`cd: "auto"` finds `RAYMAN.cue` there, and `rayman_log.txt` receives what the desktop build prints. Without RAY.LNG the
app shows where to put the data and exits (the folder exists once the app has started once). Saves and RAYMAN.CFG go
to the app's private storage (`SDL_GetPrefPath`).

Native code is always built optimised (`RelWithDebInfo`, `-O2 -g`), the debug APK included; `-PnativeBuildType=Debug`
builds it `-O0` for debugging only. Measured on a Galaxy S24 Ultra (2026-11-12, Pink Plant Woods, widescreen 100
columns per side, `pace_trace.txt`): `-O0` 36.8 fps with the native renderer at 10.6 ms per frame (median) and the
machine clock at 0.694 x real time, so gameplay and CD music ran at about 69 % speed; `-O2` 60 fps, native renderer
about 4 ms per frame, machine clock 1.003 x (display lock at 120 Hz). The native renderer draws pixel by pixel
(`render/native.c put`), so it is the cost that unoptimised builds and slower phones feel first.

Testing: an optional `args.txt` in the game folder holds command-line options of the desktop build,
whitespace-separated (e.g. `--demo 3`), read by `RaymanActivity.getArguments`. Delete it for normal play.

Diagnostics: an empty file `pace_trace.on` in the game folder makes the app write `pace_trace.txt` there (the
`RAYMAN_PACE_TRACE` line per frame, including `native_us`, the native renderer's time).

## Platform behaviour

- Landscape (`sensorLandscape`), always fullscreen. Widescreen is on by default and as wide as the screen: the number
  of extra columns per side comes from the display's aspect ratio and the pixel shape (`port_host_fit_pad`, up to
  160 per side). Square pixels (`"square_pixels": true`, the default on every platform, user decision 2026-11-12)
  show the 320x200 picture as 16:10, so a 3120x1440 phone gets 57 columns per side (100 with the original 4:3 pixel
  shape). Menus, movies and other screen-space scenes stay at 320 columns in the centre. `"widescreen": false` in
  `rayman_sdl3.json` gives the original 320-column view. A desktop build in fullscreen with widescreen on also fits
  its display; in a window it is 16:9. The game always renders 320x200: its "X mode" is the unchained (planar) VGA
  layout at 200 lines (`InitModeX` 1:046b8 sets no vertical timing), and the port runs the normal mode.
- Back key: Esc (pause in a level, back in the DOS menus), like the release's back-button listener.
- Input: the virtual pad (`platform/pad.c`): a gamepad and the touch overlay (`platform/touch.c`) set the same pad
  buttons, which become the game's keys. See TOUCH_CONTROLS.md "Port implementation".
- 64-bit `long`: Android (and Linux) are LP64. The only historical unit that declared Watcom `long` types for 32-bit
  values, `savecodec.c`, has a derivative using `int` (derive/provenance.json). All six original demos are identical
  to the reviewed golden summaries on an LP64 build (`build/port-linux`, WSL Ubuntu, gcc 15, 2026-11-12), with every
  other ctest gate passing.

## Status

- PROVEN on the x86_64 emulator (API 36, 2340x1080), touch only: logo, intro, main menu, save creation and selection,
  world map (cross + GO), level (floating stick, jump), Pause opening the DOS in-level options menu, Back returning to
  the level.
- arm64-v8a builds and links (no unresolved symbols besides libSDL3's); not yet run on a phone.
- Not yet done: the release's button artwork (the overlay draws plain discs and labels; the art can be read from the
  player's OBB, see TOUCH_CONTROLS.md open points), audio and CD music checks on a device, behaviour when the app is
  backgrounded.
