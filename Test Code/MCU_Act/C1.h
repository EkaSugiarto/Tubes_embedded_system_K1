#include <ESP32Servo.h>

Servo base, elbow, hook;

void Base_Act(void *pvParameters) {
  while (1) {
    // xSemaphoreTake(Gas, portMAX_DELAY);
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

void Hook_Act(void *pvParameters) {
  while (1) {
    if (servo_data.hook_tarik > 0) hook.write(90 + servo_data.hook_tarik);
    else if (servo_data.hook_ulur > 0) hook.write(90 - servo_data.hook_ulur);
    else hook.write(90);

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

  xTaskCreatePinnedToCore(
    Hook_Act,
    "Hook_Act",
    2048,
    NULL,
    5,
    NULL,
    1);
}
