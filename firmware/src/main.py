import uasyncio as asyncio
from wifi import connect_to_wifi, SSID, PASSWORD
from ddc import ddc
from time import sleep
import network

ddc = ddc()

if not ddc.begin():
    while True:
        print("Can't find DDC, waiting 5 seconds")
        sleep(5)
        if ddc.begin():
            break

print("Found DDC")


async def handle_client(reader, writer):
    request_line = await reader.readline()
    print(request_line)

    while await reader.readline() != b"\r\n":
        pass

    method, path, _ = request_line.decode().split()

    response = ""

    if path.startswith("/set_brightness?value="):
        try:
            value = int(path.split("=")[1])
            ddc.set_brightness(value)
            response = f"Brightness: {value}\n"
        except ValueError:
            response = "Invalid value\n"
    elif path == "/get_brightness":
        brightness = ddc.get_brightness()
        response = f"Brightness: {brightness}\n"
    else:
        response = "Invalid endpoint\n"

    writer.write("HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\n")
    writer.write(response)
    await writer.drain()
    await writer.aclose()


async def main():
    connect_to_wifi(SSID, PASSWORD)

    wlan = network.WLAN(network.STA_IF)
    ip_address = wlan.ifconfig()[0]

    server = await asyncio.start_server(handle_client, ip_address, 80)
    print(f"Server: {ip_address}:80")
    while True:
        await asyncio.sleep(3600)


asyncio.run(main())
