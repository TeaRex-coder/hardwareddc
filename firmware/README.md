# Firmware

## Building

HardwareDDC is built using Espressif's ESP32-WROOM-32E-N4.

Build environment tools:

- [esptool.py](https://github.com/espressif/esptool)
- [mklittlefs](https://github.com/earlephilhower/mklittlefs)

Bundle source code into littlefs image.

```bash
mklittlefs -c ./src -b 4096 -p 256 -s 0x200000 littlefs.bin
```

Download MicroPython image:

```bash
wget -O ESP32-v1.23.0.bin https://micropython.org/resources/firmware/ESP32_GENERIC-20240602-v1.23.0.bin
```

Combine images:

```bash
esptool.py --chip esp32 merge_bin -o combined.bin --flash_mode dio --flash_freq 40m --flash_size 4MB 0x1000 ESP320-v1.23.0-firmware.bin 0x200000 combined.bin
```

Determine serial:

```bash
ls /dev/cu.*
```

Flash image:

```bash
esptool.py --chip esp32 --port /dev/<serial port> write_flash -z 0x0 combined.bin
```
