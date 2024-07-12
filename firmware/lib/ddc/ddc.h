#ifndef DDC_H
#define DDC_H

#include <Wire.h>
#include "Arduino.h"

class DDC
{
public:
    DDC();
    bool begin();
    void setBrightness(int value);
    uint16_t getBrightness();
    void setVCP(byte op, uint16_t value);
    uint16_t getVCP(byte op);
    void setSource(uint16_t value);
    uint16_t getSource();
    void setPower(bool value);
    bool getPower();

private:
    uint8_t _I2CAddress = 0x37;
};

#endif
