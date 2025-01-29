# User Interface

HardwareDDC's goal is to provide and easy and convenient way to adjust monitor settings. Currently, HardwareDDC is controlled through an efficient macro-based UI.

## macOS

Lunar, _the defacto app for controlling monitors_, is used as HardwareDDC's macOS GUI.

1. Install [Lunar](https://static.lunar.fyi/releases/Lunar.dmg)
2. Disable all controls aside from _Network (Raspberry Pi)_
3. Reset _Network Control_

## Windows

1. Install [hardwareddc-amd64-installer.exe](https://github.com/TeaRex-coder/hardwareddc/releases) from the latest GitHub release

### Hotkeys

_User-modifiable hotkeys to come_

**Brightness:**

- ⌥⇧- = +10%
- ⌥⇧= = +10%

- ⌥⇧1 = 25%
- ⌥⇧2 = 50%
- ⌥⇧3 = 75%
- ⌥⇧4 = 100%
- ⌥⇧5 = 0%

**Input Source:**

- ⌥⇧d1 = DisplayPort 1
- ⌥⇧d2 = DisplayPort 2
- ⌥⇧h1 = HDMI 1
- ⌥⇧h2 = HDMI 2

## Linux

_Sorry, GNU/Linux_

This is coming soon, but you're a Linux user, you can figure it out.

**Example API calls to get you started:**

Specify brightness percentage:

```bash
curl -X GET "http://ddcutil.local:3485/1/brightness/<value 0-100>"
```

Get current brightness:

```bash
curl -X GET "http://ddcutil.local:3485/1/brightness"
```

Change input source:

```bash
curl -X GET "http://ddcutil.local:3485/1/input_source/<hexadecimal source>"
```
