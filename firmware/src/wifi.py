import network
from time import sleep

SSID = "ssid2.4"
PASSWORD = "psk"


def connect_to_wifi(ssid, password):
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)

    while not wlan.isconnected():
        print("Connecting...")
        sleep(1)

    print("Connected!")
    print("HardwareDDC IP:", wlan.ifconfig()[0])
