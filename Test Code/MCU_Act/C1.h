#include <ESP32Servo.h>

Servo base, elbow;

void Base_Act(void *pvParameters) {
  while (1) {
    base.write(servo_data.base);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void Elbow_Act(void *pvParameters) {
  while (1) {
    elbow.write(servo_data.elbow);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void C1S() {
  xTaskCreatePinnedToCore(
    Base_Act,
    "Base_Act",
    2048,
    NULL,
    5,
    NULL,
    1);

  xTaskCreatePinnedToCore(
    Elbow_Act,
    "Elbow_Act",
    2048,
    NULL,
    5,
    NULL,
    1);
}
