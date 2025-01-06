#include "esp32-hal.h"
#include "esp32-hal-gpio.h"

SemaphoreHandle_t AlarmOn;

String csvString = "";

template<typename T>
void addValue(T value) {
  if (csvString.length() > 0) csvString += ",";
  csvString += String(value);
}

void ReadSensor(void *pvParameters) {
  while (1) {
    if (digitalRead(33) == HIGH) {
      if (actuator_data.hook_height >= 5) {
        if (servo_data.base > 0) servo_data.base--;
      } else xSemaphoreGive(AlarmOn);

      vTaskDelay(50 / portTICK_PERIOD_MS);
    }

    if (digitalRead(25) == HIGH) {
      if (actuator_data.hook_height >= 5) {
        if (servo_data.base < 180) servo_data.base++;
      } else xSemaphoreGive(AlarmOn);

      vTaskDelay(50 / portTICK_PERIOD_MS);
    }

    if (digitalRead(26) == HIGH) {
      if (servo_data.elbow > 0) servo_data.elbow--;
      vTaskDelay(50 / portTICK_PERIOD_MS);
    }

    if (digitalRead(27) == HIGH) {
      if (servo_data.elbow < 75) servo_data.elbow++;
      vTaskDelay(50 / portTICK_PERIOD_MS);
    }

    servo_data.gripper = map(analogRead(32), 0, 4095, 0, 180);
    
    digitalRead(14) == 1 ? servo_data.hook_tarik = 90 : servo_data.hook_tarik = 0;
    digitalRead(13) == 1 ? servo_data.hook_ulur = 90 : servo_data.hook_ulur = 0;

    addValue(servo_data.base);
    addValue(servo_data.elbow);
    addValue(servo_data.gripper);
    addValue(servo_data.hook_tarik);
    addValue(servo_data.hook_ulur);
    addValue(actuator_data.hook_height);
    Serial.println(csvString);
    csvString = "";

    esp_now_send(broadcastAddress, (uint8_t *)&servo_data, sizeof(servo_data));

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void C0S() {
  AlarmOn = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(
    ReadSensor,
    "ReadSensor",
    2048,
    NULL,
    5,
    NULL,
    0);
}