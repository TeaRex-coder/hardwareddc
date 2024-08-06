setSource(source) {
    url := "http://ddcutil.local:3485/1/input_source/" source
    HttpRequest(url)
}

setBrightness(value) {
    url := "http://ddcutil.local:3485/1/brightness/" value
    HttpRequest(url)
}

adjustBrightness(amount) {
    currentBrightness := currentBrightness()
    if (currentBrightness != -1) {
        newBrightness := currentBrightness + amount
        if (newBrightness < 0) {
            newBrightness := 0
        } else if (newBrightness > 100) {
            newBrightness := 100
        }
        setBrightness(newBrightness)
    }
}

currentBrightness() {
    url := "http://ddcutil.local:3485/1/brightness"
    WebRequest := ComObjCreate("MSXML2.XMLHTTP")
    WebRequest.Open("GET", url, false)
    WebRequest.Send()
    if (WebRequest.Status = 200) {
        return WebRequest.ResponseText
    } else {
        return -1
    }
}

increaseBrightness() {
    adjustBrightness(6)
}

decreaseBrightness() {
    adjustBrightness(-6)
}

HttpRequest(url) {
    WebRequest := ComObjCreate("MSXML2.XMLHTTP")
    WebRequest.Open("GET", url, false)
    WebRequest.Send()
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
