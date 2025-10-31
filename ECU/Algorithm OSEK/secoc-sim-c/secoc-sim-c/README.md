# SecOC AES-128-CMAC Two-ECU Simulator (macOS, C)

This is a **minimal, copy‑paste‑ready** project to **simulate two ECUs (A ↔ B)** exchanging
SecOC‑protected PDUs using **AES‑128‑CMAC** with **MAC truncation = 24 bits** and **Freshness truncation = 8 bits**,
mirroring the example we've been discussing (CAN‑FD, “Profile 1”-style).

- **Crypto**: AES‑128 (ECB single‑block) from **CommonCrypto** (system library on macOS).
- **CMAC**: Implemented per **NIST SP 800‑38B / RFC 4493** (Generate_Subkey, 10* padding, MSB truncation).
- **Framing**: 8‑byte secured PDU layout: `[DataID 2][Speed 2][FV_trunc 1][MAC_trunc 3]` (big‑endian).
- **Freshness**: sender counter is 64‑bit monotonic; receiver reconstructs FV_full from 8‑bit LSB with an acceptance window.
- **Bus**: in‑memory simulated “CAN‑FD” ring buffer.

## Build (macOS)
1) Ensure Apple Command Line Tools: `xcode-select --install` (once).
2) Build:
```bash
make
```
3) Run:
```bash
./bin/secoc_sim
```

## What it does
- Runs an RFC 4493 self‑test to sanity‑check AES‑CMAC.
- ECU A sends 5 frames (`Speed` increasing); ECU B verifies each frame, reconstructs FV, and logs `ACCEPT`/`DROP`.
- Includes a simple B→A ACK demo (also SecOC‑protected) to show bi‑directional use.

## File map
- `include/` — headers (`config.h`, `common.h`, `aes_cmac.h`, `secoc.h`, `fresh.h`, `frames.h`, `bus.h`, `ecu.h`)
- `src/`     — implementations and `main.c`
- `bin/`     — build output

> Notes
> - This is **authenticity/integrity only** (no payload encryption), as per AUTOSAR **SecOC**.
> - You can change truncation sizes and layout in `include/config.h`.
> - If you prefer OpenSSL instead of CommonCrypto, replace `aes_ecb_encrypt_block()` in `src/aes_cmac.c` accordingly.
