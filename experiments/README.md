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

## Hardware DDC Aux Breakout

This board is a work in progress
