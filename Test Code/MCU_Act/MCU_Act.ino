#include "C0.h"
#include "C1.h"

#include "ESP_sekarang.h"

// MAC addr nya 24:dc:c3:c6:be:7c

void setup() {
  Serial.begin(115200);

  ESPNowS();

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  base.setPeriodHertz(50);  // standard 50 hz servo
  base.attach(16);

  elbow.setPeriodHertz(50);  // standard 50 hz servo
  elbow.attach(17);

  Serial.println("Setup complete");

  C1S();

  vTaskDelete(NULL);
}

void loop() {
}