const int trigPin1 = 21;
const int echoPin1 = 19;

#include "ESPNow.h"

#include "C0.h"
#include "C1.h"

// MAC addr 24:dc:c3:c6:be:dc

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
  gripper.attach(18);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  C0S();
  C1S();

  Serial.println("Setup complete!");

  vTaskDelete(NULL);
}

void loop() {
}
