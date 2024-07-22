#include "DDCVCP.h" 

DDCVCP ddc;

void setup() {
  Serial.begin(115200);
  delay(1000); // Give some time for the serial monitor to start
  Serial.println(F("-  Starting setup"));

  while (!ddc.begin()) {
    Serial.println(F("-  Unable to find DDC/CI. Trying again in 5 seconds."));
    delay(5000);
  }

  Serial.println(F("-  Found DDC/CI successfully."));

  // Print the current brightness value at startup
  uint16_t initialBrightness = ddc.getBrightness();
  Serial.print(F("-  Initial Brightness: "));
  Serial.println(initialBrightness);
}

void loop() {
  // Turn power on
  ddc.setPower(true);
  Serial.println(F("-  Power ON"));
  delay(3000);

  // Turn power off
  ddc.setPower(false);
  Serial.println(F("-  Power OFF"));
  delay(3000);
}
