# Releasing the port

A release is a git tag `vX.Y.Z` on `main`. The workflow `.github/workflows/port-release.yml` then builds, from a clean
checkout with no game data and no local tools, and publishes a GitHub release with:

| Asset | Built on | Contents |
|---|---|---|
| `rayman_sdl3-X.Y.Z-windows-x64.zip` | windows-2022, MSYS2 MinGW64 GCC | `rayman_sdl3.exe` (GUI program), `SDL3.dll`, README.txt, LICENSE.txt, THIRD_PARTY.txt |
| `rayman_sdl3-X.Y.Z-linux-x64.tar.gz` | ubuntu-22.04, GCC 12 (glibc 2.35) | `rayman_sdl3` (runpath `$ORIGIN`), `libSDL3.so.0`, the same text files |
| `rayman_sdl3-X.Y.Z-android.apk` | ubuntu-22.04, NDK 27.0.12077973 | arm64-v8a and x86_64; published only when signed with the release key |
| `...-symbols.zip` | | the stripped binaries' debug information (`objcopy --only-keep-debug`, linked by `.gnu_debuglink`) |

The release text is `port/docs/release-notes/vX.Y.Z.md`. A manual run of the workflow (Actions, "port release", Run
workflow) builds the same packages as artifacts without publishing.

## Steps

1. Set the version: `project(rayman_port VERSION X.Y.Z ...)` in `port/CMakeLists.txt` (the program's `--version`,
   the log's first line) and `versionName` / `versionCode` (+1) in `port/android/app/build.gradle`.
2. Write `port/docs/release-notes/vX.Y.Z.md` (what changed, known limitations).
3. Gates green locally: `ctest --test-dir build/port` and `python tools/verify.py` (0 regressions).
4. Commit, push, then `git tag vX.Y.Z` and `git push origin vX.Y.Z`.

The packages are made by `port/tools/package.py` (stripping, symbols, text files); the same command works locally.

## Pinned third-party sources

- SDL 3.4.16: `RAY_SDL3_VERSION` / `RAY_SDL3_SHA256` in `port/CMakeLists.txt`; built from the release tarball
  (`-DRAY_SDL3_FROM_SOURCE=ON`), downloaded from SDL's GitHub release when no local copy is given.
- stb_vorbis v1.22 (nothings/stb commit 1ee679ca): `RAY_STB_VORBIS_SHA256`, downloaded when no local copy is given.

## Android release key

Phones accept an update only when it is signed with the same key as the installed app, so every public APK is signed
with the project's release key, never the debug key. The key and its passwords are never in the repository.

The key (created 2026-11-13, like the maintainer's other ports' keys): `~/.android/rayman-release.jks` (alias `rayman`,
RSA 4096, certificate SHA-256 `28:55:1C:C7:B0:A1:BA:45:21:FE:E4:99:18:C7:99:3F:A4:B7:F9:66:2F:8B:60:ED:B3:BE:6C:DE:5D:4C:2C:DB`)
with `~/.android/rayman-release.properties` (storeFile, storePassword, keyAlias, keyPassword). Back up both together:
a lost key means users must uninstall (losing the app's copied game and saves) to update. `assembleRelease` signs with
it whenever the properties file exists (`-Prayman.signing=FILE` names another).

Publishing the APK: CI publishes the Windows and Linux packages; the signed APK is built locally and added to the
release, with the checksums:

```bash
python port/tools/package.py android port/android/app/build/outputs/apk/release/app-release.apk build/release
```

```bash
gh release upload vX.Y.Z build/release/rayman_sdl3-X.Y.Z-android.apk build/release/SHA256SUMS.txt
```

CI can sign instead when the repository has the secrets `RAYMAN_KEYSTORE_BASE64` (the .jks file, base64),
`RAYMAN_KEYSTORE_PASSWORD`, `RAYMAN_KEY_ALIAS` (`rayman`) and `RAYMAN_KEY_PASSWORD`; without them it signs with the
debug key and leaves the APK out of the release.
