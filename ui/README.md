# User Interface

HardwareDDC's goal is to provide and easy and convenient way to adjust monitor settings. Therefore, I prioritized an efficient macro-based UX and have not yet release a GUI.

Currently, HardwareDDC is controlled with hotkeys sending API requests.

## macOS

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

<p></p>

- ^F1- = -6%
- ^F2 = +6%

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
curl -X GET "http://hardwareddc.local/brightness?value=<value 0-100>"
```

Get current brightness:

```bash
curl -X GET "http://hardwareddc.local/brightness"
```

Change input source:

```bash
curl -X GET "http://hardwareddc.local/set_source?value=<hexadecimal source code>"
```