"""Read the player's Rayman Android (Rayman Classic 1.0.1) expansion file and decrypt its Spark2 resources.

The Android release keeps its scripts and GUI art in `main.<ver>.com.ubisoft.raymananniversary.obb` (a stored ZIP).
Every `*.spd` member is AES-128-CBC encrypted with PKCS#7 padding, a zero IV and a constant key built on the stack by
SparkUtils::DecryptBuffer (x86 libSparkApp.so 0x84de7f..0x84deca; Rijndael_cipherInit mode 2 = CBC, IV NULL).
Facts and the touch-control specification read from these files: port/docs/TOUCH_CONTROLS.md.

Output goes only to an ignored directory (build/...): the decrypted data are the player's, never tracked.

    python port/tools/android_obb.py list OBB [--grep TEXT]
    python port/tools/android_obb.py extract OBB OUT_DIR [--prefix Spark2Projects/RaymanGameManager/] [--scripts-only]
    python port/tools/android_obb.py selftest
"""
import argparse
import os
import sys
import zipfile

KEY = bytes([1, 2, 3, 4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2, 1, 10])

# --- AES-128 decryption (FIPS-197), stdlib only ---------------------------------------------------------------
_SBOX = [0] * 256
_INV = [0] * 256


def _init_sbox():
    p = q = 1
    while True:
        p = p ^ ((p << 1) & 0xFF) ^ (0x1B if p & 0x80 else 0)
        q ^= q << 1
        q ^= q << 2
        q ^= q << 4
        q &= 0xFF
        if q & 0x80:
            q ^= 0x09
        x = q ^ ((q << 1 | q >> 7) & 0xFF) ^ ((q << 2 | q >> 6) & 0xFF) ^ ((q << 3 | q >> 5) & 0xFF) \
            ^ ((q << 4 | q >> 4) & 0xFF) ^ 0x63
        _SBOX[p] = x
        if p == 1:
            break
    _SBOX[0] = 0x63
    for i, v in enumerate(_SBOX):
        _INV[v] = i


_init_sbox()


def _xt(a):
    return ((a << 1) ^ 0x1B) & 0xFF if a & 0x80 else a << 1


def _mul(a, b):
    r = 0
    while b:
        if b & 1:
            r ^= a
        a = _xt(a)
        b >>= 1
    return r


_M9 = [_mul(i, 9) for i in range(256)]
_M11 = [_mul(i, 11) for i in range(256)]
_M13 = [_mul(i, 13) for i in range(256)]
_M14 = [_mul(i, 14) for i in range(256)]


def _expand(key):
    w = list(key)
    rcon = 1
    for i in range(4, 44):
        t = w[(i - 1) * 4:i * 4]
        if i % 4 == 0:
            t = [_SBOX[t[1]] ^ rcon, _SBOX[t[2]], _SBOX[t[3]], _SBOX[t[0]]]
            rcon = _xt(rcon)
        w += [w[(i - 4) * 4 + j] ^ t[j] for j in range(4)]
    return [w[r * 16:(r + 1) * 16] for r in range(11)]


_ROUND_KEYS = _expand(KEY)


def _decrypt_block(b):
    s = [b[i] ^ _ROUND_KEYS[10][i] for i in range(16)]
    for rnd in range(9, -1, -1):
        # inverse ShiftRows + inverse SubBytes (column-major state)
        s = [_INV[s[((c - r) % 4) * 4 + r]] for c in range(4) for r in range(4)]
        k = _ROUND_KEYS[rnd]
        s = [s[i] ^ k[i] for i in range(16)]
        if rnd:
            t = []
            for c in range(4):
                a0, a1, a2, a3 = s[c * 4:c * 4 + 4]
                t += [_M14[a0] ^ _M11[a1] ^ _M13[a2] ^ _M9[a3],
                      _M9[a0] ^ _M14[a1] ^ _M11[a2] ^ _M13[a3],
                      _M13[a0] ^ _M9[a1] ^ _M14[a2] ^ _M11[a3],
                      _M11[a0] ^ _M13[a1] ^ _M9[a2] ^ _M14[a3]]
            s = t
    return s


def decrypt(data):
    """AES-128-CBC, zero IV, PKCS#7 padding (Rijndael_padDecrypt)."""
    if len(data) % 16:
        raise ValueError("ciphertext length is not a multiple of 16")
    out = bytearray()
    prev = bytes(16)
    for off in range(0, len(data), 16):
        blk = data[off:off + 16]
        p = _decrypt_block(blk)
        out += bytes(p[i] ^ prev[i] for i in range(16))
        prev = blk
    pad = out[-1] if out else 0
    if not 1 <= pad <= 16 or out[-pad:] != bytes([pad]) * pad:
        raise ValueError("bad padding (wrong key or not an encrypted Spark2 file)")
    return bytes(out[:-pad])


SCRIPT_MARKERS = (b"Class", b"--", b"global", b"\xef\xbb\xbf")


def is_script(plain):
    return plain.lstrip()[:12].startswith(SCRIPT_MARKERS)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("list")
    p.add_argument("obb")
    p.add_argument("--grep", help="only members whose decrypted text contains this")
    p = sub.add_parser("extract")
    p.add_argument("obb")
    p.add_argument("out")
    p.add_argument("--prefix", default="")
    p.add_argument("--scripts-only", action="store_true")
    sub.add_parser("selftest")
    a = ap.parse_args(argv)

    if a.cmd == "selftest":
        # FIPS-197 appendix C.1 vector, decrypted with its own key schedule.
        global _ROUND_KEYS
        saved = _ROUND_KEYS
        _ROUND_KEYS = _expand(bytes(range(16)))
        got = bytes(_decrypt_block(bytes.fromhex("69c4e0d86a7b0430d8cdb78070b4c55a")))
        _ROUND_KEYS = saved
        ok = got == bytes.fromhex("00112233445566778899aabbccddeeff")
        print("AES-128 FIPS-197 C.1:", "ok" if ok else "FAIL " + got.hex())
        return 0 if ok else 1

    z = zipfile.ZipFile(a.obb)
    names = [n for n in z.namelist() if n.endswith(".spd")]
    if a.cmd == "list":
        for n in names:
            if a.grep:
                try:
                    if a.grep.encode() not in decrypt(z.read(n)):
                        continue
                except ValueError:
                    continue
            print(n)
        return 0

    out_root = os.path.abspath(a.out)
    count = 0
    for n in names:
        if not n.startswith(a.prefix):
            continue
        plain = decrypt(z.read(n))
        if a.scripts_only and not is_script(plain):
            continue
        dest = os.path.abspath(os.path.join(out_root, n[:-4]))
        if not dest.startswith(out_root + os.sep):
            raise SystemExit("refusing member outside the output directory: " + n)
        os.makedirs(os.path.dirname(dest), exist_ok=True)
        with open(dest, "wb") as f:
            f.write(plain)
        count += 1
    print(f"{count} files decrypted to {out_root}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
