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

Creating it (once, by the maintainer; keep the file and passwords safe - a lost key means users must uninstall to
update):

```bash
keytool -genkeypair -v -keystore rayman-release.jks -alias rayman -keyalg RSA -keysize 4096 -validity 36500
```

GitHub repository secrets used by the workflow: `RAYMAN_KEYSTORE_BASE64` (the .jks file, base64), `RAYMAN_KEYSTORE_PASSWORD`,
`RAYMAN_KEY_ALIAS` (`rayman`), `RAYMAN_KEY_PASSWORD`. Without them the workflow signs with the debug key and leaves the
APK out of the release. Local release builds use the same names as Gradle properties or environment variables
(`port/android/app/build.gradle`).
