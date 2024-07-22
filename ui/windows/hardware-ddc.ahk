setSource(value) {
    url := "http://hardwareddc.local/set_source?value=" value
    Http := ComObjCreate("WinHttp.WinHttpRequest.5.1")
    Http.Open("GET", url, false)
    Http.Send()
}

setBrightness(value) {
    url := "http://hardwareddc.local/brightness?value=" value
    Http := ComObjCreate("WinHttp.WinHttpRequest.5.1")
    Http.Open("GET", url, false)
    Http.Send()
}

getBrightness() {
    url := "http://hardwareddc.local/brightness"
    Http := ComObjCreate("WinHttp.WinHttpRequest.5.1")
    Http.Open("GET", url, false)
    Http.Send()
    response := Http.ResponseText
    RegExMatch(response, """current_brightness"":\s*(\d+)", match)
    return match1
}

adjustBrightness(delta) {
    currentBrightness := getBrightness()
    newBrightness := currentBrightness + delta
    newBrightness := Max(0, Min(100, newBrightness))
    setBrightness(newBrightness)
}

increaseBrightness() {
    adjustBrightness(6)
}

decreaseBrightness() {
    adjustBrightness(-6)
}

^#!1::setSource(0x0f)
^#!2::setSource(0x10)
^#!3::setSource(0x11)
^#!4::setSource(0x12)

^!1::setBrightness(25)
^!2::setBrightness(50)
^!3::setBrightness(75)
^!4::setBrightness(100)
^!5::setBrightness(0)

^f1::decreaseBrightness()
^f2::increaseBrightness()