const int trigPin = 26;
const int echoPin = 25;

#include "ESPNow.h"

#include "C0.h"
#include "C1.h"

void setup() {
  Serial.begin(115200);

  // Setup ESP-NOW
  ESPNowS();

  // Set timer untuk generate sinyal PWM pada pin-pin servo
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Setup pin yang digunakan untuk servo (dengan f = 50 Hz (default))
  base.setPeriodHertz(50);
  base.attach(4);

  elbow.setPeriodHertz(50);
  elbow.attach(16);

  hook.setPeriodHertz(50);
  hook.attach(17);

  gripper.setPeriodHertz(50);
  gripper.attach(5);

  // Setup pin sensor ultrasonik
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Berisi setup fungsi RTOS yang dijalankan di Core 0 ESP32
  C0S();
  // Berisi setup fungsi RTOS yang dijalankan di Core 1 ESP32
  C1S();

  //Untuk menghapus loop task untuk meminimalisir gangguan pada saat sistem sedang idle
  vTaskDelete(NULL);
}

void loop() {
}
