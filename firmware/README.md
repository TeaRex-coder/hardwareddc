# Firmware

## Building

HardwareDDC is built using Espressif's ESP32-WROOM-32E-N4.

Build environment tools:

- [esptool.py](https://github.com/espressif/esptool)
- [PlatformIO](https://github.com/platformio)

### Generate image

```bash
esptool.py --chip esp32 merge_bin -o ./public/image.bin --flash_mode dio --flash_size 4MB 0x1000 ./firmware/.pio/build/esp32dev/bootloader.bin 0x8000 ./firmware/.pio/build/esp32dev/partitions.bin 0x10000 ./firmware/.pio/build/esp32dev/firmware.bin
```

### Flash Image

```bash
esptool.py --chip esp32 --port <serial port> write_flash 0x0 image.bin
```
