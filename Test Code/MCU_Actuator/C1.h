#include <ESP32Servo.h>

Servo base, elbow, hook, gripper;

void BaseServo(void *pvParameters) {
  while (1) {
    xSemaphoreTake(RunBaseMotor, portMAX_DELAY);
    base.write(servo_data.base);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void ElbowServo(void *pvParameters) {
  while (1) {
    elbow.write(servo_data.elbow);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void HookServo(void *pvParameters) {
  while (1) {
    if (servo_data.hook_tarik > 0) hook.write(90 + servo_data.hook_tarik);
    else if (servo_data.hook_ulur > 0) hook.write(90 - servo_data.hook_ulur);
    else hook.write(90);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void GripperServo(void *pvParameters) {
  while (1) {
    gripper.write(servo_data.gripper);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void C1S() {
  xTaskCreatePinnedToCore(
    BaseServo,
    "BaseServo",
    2048,
    NULL,
    5,
    NULL,
    1);

  xTaskCreatePinnedToCore(
    ElbowServo,
    "ElbowServo",
    2048,
    NULL,
    5,
    NULL,
    1);

  xTaskCreatePinnedToCore(
    HookServo,
    "HookServo",
    2048,
    NULL,
    5,
    NULL,
    1);

  xTaskCreatePinnedToCore(
    GripperServo,
    "GripperServo",
    2048,
    NULL,
    5,
    NULL,
    1);
}
