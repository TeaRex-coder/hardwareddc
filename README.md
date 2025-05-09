<h1 align="center">HardwareDDC</h1>
<h4 align="center">A custom PCB that makes adjusting monitor settings easy and convenient</h4>

<!-- Horizontal layout with 3 images -->
<p align="center">
  <img src="./images/boardfront.webp" alt="front-view" width="250" height="250">
  <img src="./images/assembled.webp" alt="assembled" width="250" height="250">
  <img src="./images/boardtop.webp" alt="top-view" width="250" height="250">
</p>

## What is DDC?

DDC/CI (Display Data Channel Command Interface) lets you control monitors with software instead of navigating the nipple or rear buttons. In a perfect world, we could control all monitors with DDC, but unfortunately, that's not the case...

- Some monitors simply don't support DDC :(
- Some monitors like Samsung's G9 ultrawides support DDC only on HDMI
- \*nix (i.e. macOS & Linux) users of DisplayLink don't have DDC support
- Your monitor only supports DDC through HDMI (not DisplayPort)

If you're in one of those last three situations HardwareDDC might be for you! It uses an ESP32 to send raw I<sup>2</sup>C DDC signals to your monitor's HDMI port while another port is used for video.

## Getting Started

HardwareDDC features an ESP32 with reset and boot buttons, an HDMI connector exposing GND, SDA and SCL, and it's powered by USB-C which also exposes the UART console. Optionally, it can be powered by 5V directly through the GPIO pins, and additional GPIO pins are available for debugging and future expandability.

The PCB and the 3D printed ABS case are available on the [website](https://hardwareddc.alexandreboutoille.com), and the files can be found [here](./hardware/README.md).

Once you have your board, you can flash the firmware with the [web installer](https://hardwareddc.alexandreboutoille.com).

Controlling HardwareDDC is done through hotkeys, instructions can be found [here](./ui/README.md)

---

## Sponsors

<img src="./images/pcbwaylogo.svg " alt="PCBWay Logo" width="300"/>

Thank you PCBWay for sponsoring this project providing PCB prototyping <3

<details>
<summary>Review & Opinions</summary>

TL;DR Would I recommend PCBWay's PCB service? Yes!

It's impossible for me to provide an unbiased opinion as PCBWay has provided boards free of charge to help me work on this project. However, my experience was overall positive. Starting with their marketing rep Liam, he offered a partnership respectfully and fairly. When placing the order, the team helped me, a novice to PCB design and PCBWay's services, follow some best practices regarding board design and ordering. Finally, the production tracker helped keep my curious mind informed and the assembled product functioned as intended.

When researching PCBWay's brand before agreeing to a partnership, I noticed a customer expressing disappointment in the assembly of an HDMI connector. I took this with a grain of salt, as the dissatisfied customers are generally the loudest, but it was still a concern. The boards I received had well-soldered DisplayPort connector pins to a level as good, if not better than I could do by hand.

<p align="center">
  <img src="./images/pcbwaySolder.webp" alt="PCBWay Logo" width="720"/>
</p>

I hope the experience I received is representative of any regular PCBWay customer despite PCBWay expecting a review. If you need PCB prototyping for your project, consider giving them a go :)

</details>

## Future Ideas

- [x] Improve Microsoft Windows UX/Create Proper User Interface
  - [ ] Add more information to GUI (e.g. connection status)
  - [ ] Enable user-modifiable hotkeys
- [ ] Support Multiple Monitors
- [x] Convert DisplayPort to VGA for a non-network-based approach

## Extras

Shoutout to these projects that make HardwareDDC possible:

- [ESP Web Tools](https://github.com/esphome/esp-web-tools)
- [Improv Wi-Fi](https://github.com/jnthas/Improv-WiFi-Library)
- [DDC/CI VCP library](https://github.com/tttttx2/ddcvcp)
