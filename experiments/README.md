# HardwareDDC Experiments

_I do a lot of work behind the scene to learn about DDC-related tech. In this directory, I want to share some of my fun HardwareDDC experiments._

## HardwareDDC Aux V1.0

This board uses the [PTN3392](https://www.nxp.com/docs/en/data-sheet/PTN3392.pdf) DisplayPort to VGA IC to convert DP Aux into i2c

Goals:

- Reduce latency
- No client software

[Schematic](./hardwareddc-aux-v1.0/hardwareddc-aux-v1.0.pdf)

[ECAD files](./hardwareddc-aux-v1.0/hardwareddc-aux-v1.0.zip)

Learnings:

- This solution is jank and pretty unstable
- Next time, I'll try using [TI's M-LVDS](https://www.ti.com/lit/ug/tidu599/tidu599.pdf) transceivers to interact with the Aux channel

## HardwareDDC Aux 2.0 "Breakout"

This board _attempts_ to "sniff" the DisplayPort auxiliary channel using [TI's SN65MLVD200AD](https://www.ti.com/lit/ds/symlink/sn65mlvd200a.pdf).

What I learned:

- How to better route high-speed differential pairs (DisplayPort video signal)
- Improved understanding of DisplayPort AUX

What didn't work:

- Sniffing the AUX channel

I think it didn't work because of my false assumptions of the physical hardware layout of DisplayPort AUX on both the source and sink. Possible also because of the assumptions I made before understanding the communication protocol.

In the future, I'll be trying using a more traditional Hardware layout as suggested in the DisplayPort 1.2 standards document to using a more MitM-like approach inspired by [JX5S' work](https://github.com/JX5S/DP_aux_emulator).

## HardwareDDC AUX 2.1 "MitM"

WIP
