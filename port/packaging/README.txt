Rayman (native port) {version}
==============================

A native port of the DOS game Rayman (PC, version 1.21 - the version GOG sells as part of "Rayman Forever"),
rebuilt from a reconstruction of the original program's source code. It plays your own copy of the game: no game
files are included. Source code and documentation: https://github.com/missingno7/rayman_recon

WINDOWS
  Copy rayman_sdl3.exe and SDL3.dll into the game folder - in GOG's Rayman Forever that is the "Rayman" folder
  (the one with RAY.LNG) - and start rayman_sdl3.exe. The original RAYMAN.EXE stays untouched next to it.

LINUX
  Copy rayman_sdl3 and libSDL3.so.0 into the game folder (the one with RAY.LNG; in GOG's Rayman Forever the
  "Rayman" folder) and start ./rayman_sdl3 from there.

ANDROID
  Install the APK. On the first start the app asks for your game folder (copy GOG's "Rayman Forever" folder, or a
  Rayman 1.21 game folder, to the phone first) and copies what the game needs - about 190 MB for GOG - into its
  own storage. Touch controls follow the official Android release; gamepads work too.

MUSIC AND THE CD
  The game's music is CD audio. The port finds, in this order: an original Rayman CD image in the game folder or
  the folder above it (any .cue sheet with its .bin), a Rayman CD in a drive (a real disc or a mounted image,
  Windows), then GOG's own CD (game.ins with game.gog and the Music folder, one level above the game). Without any
  CD the full game plays without music.
  GOG's music files cut three tracks short (the Jungle, Picture City and Caves of Skops themes play for about a
  minute, then silence until the game restarts them; the Jungle theme is also a different piece). The original
  CD has the full tracks, so it is preferred when present.

CINEMATICS
  GOG's release has no intro or ending movie. The port plays them when INTRO.DAT and CONCLU.DAT are in the game
  folder (from an original CD install) or on an inserted original CD.

SAVES AND SETTINGS
  In a GOG install the saves go to GOG's cloud_saves folder, shared with GOG's own launcher; otherwise to your
  user's application-data folder. Settings are in rayman_sdl3.json next to the program (created on the first
  start). In the game: Options > Graphics Details > Video mode opens the Display screen: widescreen (off, 16:9,
  21:9, fit to the window), square or original 4:3 pixels, fullscreen (also Alt+Enter).

KNOWN LIMITATIONS (version {version})
  - Only Rayman PC version 1.21 (GOG) is supported. Other versions, such as the v1.12 CD release, show a message
    instead of starting.
  - Not every part of the game has been checked automatically yet: boss fights, level endings, timed levels and the
    victory sequence are not covered by the tests.
  - Android: the touch controls are also shown during the attract-mode demos; a corrupted edge was once seen around
    the firefly light in a dark level and is not explained yet.
  - Problems: the program writes rayman_log.txt in the game folder (Android: in the app's folder). Please attach it
    to bug reports at https://github.com/missingno7/rayman_recon/issues

LICENCES
  The port and the reconstructed source are free software under the GNU GPL, version 2 or later (LICENSE.txt).
  Rayman, its name, data and original program belong to Ubisoft; you need your own copy of the game.
  Third-party code: THIRD_PARTY.txt.
