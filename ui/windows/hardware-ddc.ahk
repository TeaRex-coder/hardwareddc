setSource(source) {
    url := "http://ddcutil.local:3485/1/input_source/" source
    HttpRequest(url)
}

setBrightness(value) {
    url := "http://ddcutil.local:3485/1/brightness/" value
    HttpRequest(url)
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