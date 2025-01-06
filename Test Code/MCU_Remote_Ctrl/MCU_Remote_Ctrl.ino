#include "ESPNow.h"
#include "C0.h"
#include "C1.h"

void setup() {
  Serial.begin(115200);

  // Setup ESP-NOW
  ESPNowS();

  // Setup pin yang digunakan pada remote control
  pinMode(32, INPUT);

  pinMode(33, INPUT_PULLDOWN);
  pinMode(25, INPUT_PULLDOWN);
  pinMode(26, INPUT_PULLDOWN);
  pinMode(27, INPUT_PULLDOWN);

  pinMode(14, INPUT_PULLDOWN);
  pinMode(13, INPUT_PULLDOWN);

  pinMode(23, OUTPUT);

  // Berisi setup fungsi RTOS yang dijalankan di Core 0 ESP32
  C0S();
  // Berisi setup fungsi RTOS yang dijalankan di Core 1 ESP32
  C1S();

  //Untuk menghapus loop task untuk meminimalisir gangguan pada saat sistem sedang idle
  vTaskDelete(NULL);
}

void loop() {
}
