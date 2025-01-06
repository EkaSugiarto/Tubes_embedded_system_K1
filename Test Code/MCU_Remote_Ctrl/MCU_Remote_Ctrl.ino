#include "ESPNow.h"
#include "C0.h"
#include "C1.h"

void setup() {
  Serial.begin(115200);
  
  ESPNowS();

  pinMode(32, INPUT);

  pinMode(33, INPUT_PULLDOWN);
  pinMode(25, INPUT_PULLDOWN);
  pinMode(26, INPUT_PULLDOWN);
  pinMode(27, INPUT_PULLDOWN);

  pinMode(14, INPUT_PULLDOWN);
  pinMode(13, INPUT_PULLDOWN);

  pinMode(23, OUTPUT);

  C0S();
  C1S();

  vTaskDelete(NULL);
}

void loop() {
}
