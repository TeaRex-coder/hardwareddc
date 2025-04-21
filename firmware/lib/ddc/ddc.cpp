#include "Arduino.h"
#include "ddc.h"

DDC::DDC() {}

bool DDC::begin() {
    Wire.begin();
    Wire.beginTransmission(_I2CAddress);
    if (Wire.endTransmission() == 0) {
        return true;
    }
    return false;
}

void DDC::setVCP(byte op, uint16_t value) {
    Wire.beginTransmission(_I2CAddress);
    Wire.write(0x51);
    Wire.write(0x84);
    Wire.write(0x03);
    Wire.write(op);
    Wire.write(byte(value >> 8));
    Wire.write(byte(value));
    Wire.write((_I2CAddress << 1) ^ 0x51 ^ 0x84 ^ 0x03 ^ op ^ byte(value >> 8) ^
               byte(value)); // XOR checksum. Include all bytes sent, including slave address!
    Wire.endTransmission();
}

uint16_t DDC::getVCP(byte op) {
    Wire.beginTransmission(_I2CAddress);
    Wire.write(0x51);
    Wire.write(0x82);
    Wire.write(0x01);
    Wire.write(op);
    Wire.write((_I2CAddress << 1) ^ 0x51 ^ 0x82 ^ 0x01 ^
               op); // XOR checksum. Include all bytes sent, including slave address!
    Wire.endTransmission();
    delay(40);
    Wire.requestFrom(_I2CAddress, (uint8_t)12);
    byte response[12];
    int i = 0;
    while (Wire.available()) // slave may send less than requested
    {
        byte c = Wire.read(); // receive a byte as character
        response[i] = c;
        i++;
        // Serial.printf("%x ",c);         // print the character
    }
    return (response[8] << 8) + response[9];
}

void DDC::setBrightness(int value) {
    if (value > 100) {
        value = 100;
    }
    setVCP(0x10, value);
}

uint16_t DDC::getBrightness() {
    return getVCP(0x10);
}

void DDC::setContrast(int value) {
    if (value > 100) {
        value = 100;
    }
    setVCP(0x12, value);
}

uint16_t DDC::getContrast() {
    return getVCP(0x12);
}

// 0x01:VGA, 0x03:DVI, 0x0f:DP
void DDC::setSource(uint16_t value) {
    setVCP(0x60, value);
}

uint16_t DDC::getSource() {
    return getVCP(0x60);
}

// true:on, false:suspend
void DDC::setPower(bool value) {
    if (value) {
        setVCP(0xD6, 0x01);
    } else {
        setVCP(0xD6, 0x03);
    }
}

bool DDC::getPower() {
    byte power = getVCP(0xD6);
    if (power == 0x01) {
        return true;
    }
    return false;
}

void DDC::getEDID(uint8_t* edid) {
    for (int i = 0; i < 128; i += 16) {
        Wire.beginTransmission(0x50);
        Wire.write(i);
        Wire.endTransmission();
        Wire.requestFrom(0x50, 16);
        for (int j = 0; j < 16 && Wire.available(); j++) {
            edid[i + j] = Wire.read();
        }
    }
}

String DDC::getMfg() {
    uint8_t edid[128];
    getEDID(edid);

    char mfg[4] = {0};
    uint8_t byte1 = edid[8];
    uint8_t byte2 = edid[9];

    mfg[0] = ((byte1 & 0x7C) >> 2) + '@';
    mfg[1] = ((byte1 & 0x03) << 3) + ((byte2 & 0xE0) >> 5) + '@';
    mfg[2] = (byte2 & 0x1F) + '@';

    return String(mfg);
}

String DDC::getModel() {
    uint8_t edid[128];
    getEDID(edid);

    for (int i = 54; i < 128; i += 18) {
        if (edid[i] == 0x00 && edid[i + 1] == 0x00 && edid[i + 2] == 0x00 && edid[i + 3] == 0xFC) {
            String model = "";
            for (int j = i + 5; j < i + 18; j++) {
                if (edid[j] == 0x0A || edid[j] == 0x20) {
                    break;
                }
                model += (char)edid[j];
            }
            return model;
        }
    }

    return "";
}

String DDC::getProduct() {
    uint8_t edid[128];
    getEDID(edid);

    uint16_t productCode = (edid[11] << 8) | edid[10];

    return String(productCode);
}

String DDC::getProductSerial() {
    uint8_t edid[128];
    getEDID(edid);

    for (int i = 54; i < 128; i += 18) {
        if (edid[i] == 0x00 && edid[i + 1] == 0x00 && edid[i + 2] == 0x00 && edid[i + 3] == 0xFF) {
            String serial = "";
            for (int j = i + 5; j < i + 18; j++) {
                if (edid[j] == 0x0A || edid[j] == 0x20) {
                    break;
                }
                serial += (char)edid[j];
            }
            return serial;
        }
    }

    return "";
}

String DDC::getSerial() {
    uint8_t edid[128];
    getEDID(edid);

    uint32_t serialNumber =
        (edid[0x0F] << 24) | (edid[0x0E] << 16) | (edid[0x0D] << 8) | edid[0x0C];

    char hexString[11];
    sprintf(hexString, "0x%08X", serialNumber);

    return String(hexString);
}

uint32_t DDC::getSerialDecimal() {
    uint8_t edid[128];
    getEDID(edid);

    uint32_t serialNumber =
        (edid[0x0F] << 24) | (edid[0x0E] << 16) | (edid[0x0D] << 8) | edid[0x0C];

    return serialNumber;
}

uint16_t DDC::getYear() {
    uint8_t edid[128];
    getEDID(edid);

    uint8_t yearOffset = edid[17];
    return 1990 + yearOffset;
}

uint8_t DDC::getWeek() {
    uint8_t edid[128];
    getEDID(edid);

    return edid[16];
}

String DDC::getVCP() {
    uint16_t version = getVCP(0xDF);
    uint8_t major = (version >> 8) & 0xFF;
    uint8_t minor = version & 0xFF;

    String versionString = String(major) + "." + String(minor);

    if (versionString == "2.0" || versionString == "2.1" || versionString == "3.0" ||
        versionString == "2.2") {
        return versionString;
    } else {
        return "Detection failed";
    }
}
