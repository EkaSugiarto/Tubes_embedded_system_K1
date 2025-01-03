struct {
  int base, elbow, hook_tarik, hook_ulur;
} servo_data;

// SemaphoreHandle_t Gas;

void Gas_gak(void *pvParameters) {
  while (1) {
    Serial.println(servo_data.base);
    Serial.println(servo_data.elbow);
    Serial.println(servo_data.hook_tarik);
    Serial.println(servo_data.hook_ulur);
    // if (servo_data.Alt >= 50) xSemaphoreGive(Gas);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void C0S() {
  // Gas = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(
    Gas_gak,
    "Gas_gak",
    2048,
    NULL,
    5,
    NULL,
    1);
}