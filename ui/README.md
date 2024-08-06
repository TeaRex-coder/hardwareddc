# User Interface

HardwareDDC's goal is to provide and easy and convenient way to adjust monitor settings. Therefore, I prioritized an efficient macro-based UX and have not yet released a first-party GUI.

Currently, HardwareDDC is controlled with hotkeys sending API requests.

## macOS

### Lunar

Lunar, _the defacto app for controlling monitors_, can be used as HardwareDDC's interface instead of Hammerspoon for those preferring a GUI.

1. Install [Lunar](https://static.lunar.fyi/releases/Lunar.dmg)
2. Disable all controls aside from _Network (Raspberry Pi)_
3. Reset _Network Control_

### Hammerspoon

1. Install [Hammerspoon](https://www.hammerspoon.org)
2. Add [hardware-ddc.lua](./macos/hardware-ddc.lua) to your Hammerspoon config `~/.hammerspoon/init.lua`

```lua
local env = require("hardware-ddc")
```

### Hotkeys

_Edit macros in [hardware-ddc.lua](./macos/hardware-ddc.lua)_

**Brightness:**

- ^⌘5 = 0%
- ^⌘1 = 25%
- ^⌘2 = 50%
- ^⌘3 = 75%
- ^⌘4 = 100%

<p></p>

- fn + F1 = -6%
- fn + F2 = +6%

**Input Source:**

- ^⌘h1 HDMI 1
- ^⌘h2 HDMI 2
- ^⌘d1 DisplayPort 1
- ^⌘d2 DisplayPort 2

## Windows

1. Install [AutoHotkey](https://www.autohotkey.com)
2. Create [hardware-ddc.ahk](./windows/hardware-ddc.ahk) shortcut to run at startup in `%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup`

### Hotkeys

_Edit macros in [hardware-ddc.ahk](./windows/hardware-ddc.ahk)_

**Brightness:**

- ^⌥1 = 25%
- ^⌥2 = 50%
- ^⌥3 = 75%
- ^⌥4 = 100%
- ^⌥5 = 0%

**Input Source:**

- ^⌥h1 HDMI 1
- ^⌥h2 HDMI 2
- ^⌥d1 DisplayPort 1
- ^⌥d2 DisplayPort 2

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
