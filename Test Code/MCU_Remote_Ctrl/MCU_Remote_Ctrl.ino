#include "ESPNow.h"
#include "C0.h"

void setup() {
  Serial.begin(115200);
  ESPNowS();

  C0S();

  vTaskDelete(NULL);
}

void loop() {
}
