function setSource(value, name)
    local url = "http://hardwareddc.local/set_source?value=" .. value
    hs.http.asyncGet(url, nil, function(status, body, headers)
    end)
end

function setDP1()
    setSource("0x0f")
end

function setDP2()
    setSource("0x10")
end

function setHDMI1()
    setSource("0x11")
end

function setHDMI2()
    setSource("0x12")
end

function setBrightness(value)
    local url = "http://hardwareddc.local/brightness?value=" .. value
    hs.http.asyncGet(url, nil, function(status, body, headers)
    end)
end

function setBrightness0()
    setBrightness("0")
end

function setBrightness25()
    setBrightness("25")
end

function setBrightness50()
    setBrightness("50")
end

function setBrightness75()
    setBrightness("75")
end

function setBrightness100()
    setBrightness("100")
end

function getBrightness(callback)
    hs.http.asyncGet("http://hardwareddc.local/brightness", nil, function(_, body)
        local brightness = hs.json.decode(body).current_brightness
        callback(brightness)
    end)
end

function adjustBrightness(delta)
    getBrightness(function(currentBrightness)
        local newBrightness = math.max(0, math.min(100, currentBrightness + delta))
        setBrightness(newBrightness)
    end)
end

function increaseBrightness()
    adjustBrightness(6)
end

function decreaseBrightness()
    adjustBrightness(-6)
end

hyper = {"cmd", "alt", "ctrl"}

hs.hotkey.bind(hyper, "1", setDP1)
hs.hotkey.bind(hyper, "2", setDP2)
hs.hotkey.bind(hyper, "3", setHDMI1)
hs.hotkey.bind(hyper, "4", setHDMI2)

hs.hotkey.bind({"cmd", "ctrl"}, "1", setBrightness25)
hs.hotkey.bind({"cmd", "ctrl"}, "2", setBrightness50)
hs.hotkey.bind({"cmd", "ctrl"}, "3", setBrightness75)
hs.hotkey.bind({"cmd", "ctrl"}, "4", setBrightness100)
hs.hotkey.bind({"cmd", "ctrl"}, "5", setBrightness0)

hs.hotkey.bind({"fn"}, "F1", decreaseBrightness)
hs.hotkey.bind({"fn"}, "F2", increaseBrightness)
