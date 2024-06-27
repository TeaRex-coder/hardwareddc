from machine import I2C, Pin
from time import sleep


class ddc:
    def __init__(self):
        self.i2c_address = 0x37
        self.i2c = I2C(0, scl=Pin(22), sda=Pin(21), freq=100000)

    def begin(self):
        try:
            self.i2c.writeto(self.i2c_address, b"")
            return True
        except OSError:
            return False

    def set_vcp(self, op, value):
        data = bytearray(7)
        data[0] = 0x51
        data[1] = 0x84
        data[2] = 0x03
        data[3] = op
        data[4] = value >> 8
        data[5] = value & 0xFF
        checksum = (self.i2c_address << 1) ^ 0x51 ^ 0x84 ^ 0x03 ^ op ^ data[4] ^ data[5]
        data[6] = checksum
        self.i2c.writeto(self.i2c_address, data)

    def get_vcp(self, op):
        data = bytearray(5)
        data[0] = 0x51
        data[1] = 0x82
        data[2] = 0x01
        data[3] = op
        checksum = (self.i2c_address << 1) ^ 0x51 ^ 0x82 ^ 0x01 ^ op
        data[4] = checksum
        self.i2c.writeto(self.i2c_address, data)
        sleep(0.04)
        response = self.i2c.readfrom(self.i2c_address, 12)
        return (response[8] << 8) + response[9]

    def set_brightness(self, value):
        if value > 100:
            value = 100
        self.set_vcp(0x10, value)

    def get_brightness(self):
        return self.get_vcp(0x10)
