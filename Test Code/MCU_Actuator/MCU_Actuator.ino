const int trigPin = 26;
const int echoPin = 25;

#include "ESPNow.h"

#include "C0.h"
#include "C1.h"

void setup() {
  Serial.begin(115200);

  ESPNowS();

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  base.setPeriodHertz(50);
  base.attach(4);

  elbow.setPeriodHertz(50);
  elbow.attach(16);

  hook.setPeriodHertz(50);
  hook.attach(17);

  gripper.setPeriodHertz(50);
  gripper.attach(5);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  C0S();
  C1S();

  Serial.println("Setup complete!");

  vTaskDelete(NULL);
}

void loop() {
}
