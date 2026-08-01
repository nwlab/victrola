Project: STM8S208 minimal demo

What I added:
- `src/main.c`: Minimal app that uses the internal HSI clock and toggles `PD0`.
- `platformio.ini`: updated to set `upload_protocol = stlink`.

Build and flash (PlatformIO):

1. Build:

```bash
pio run -e stm8s208s6
```

2. Build + upload (uses `upload_protocol` in platformio.ini):

```bash
pio run -e stm8s208s6 -t upload
```

Alternative: produce a binary and flash with `stm8flash`:

```bash
pio run -e stm8s208s6 -t build
# output binary will be in .pio/build/stm8s208s6/firmware.bin (or check .elf/.hex)
stm8flash -c usb -p stm8s208s6 -w .pio/build/stm8s208s6/firmware.bin
```

Notes:
- The code uses the Standard Peripheral Library (SPL) provided by PlatformIO when `framework = spl`.
- The LED pin is `PD0` in the example — change to match your board/hardware.
- If you use a different programmer (e.g. `stm8sbootloader` or `stm8flash`), adjust `platformio.ini` accordingly.