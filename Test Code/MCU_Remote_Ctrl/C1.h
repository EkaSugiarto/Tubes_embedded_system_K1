void Alarm(void *pvParameters) {
  while (1) {
    xSemaphoreTake(AlarmOn, portMAX_DELAY);

    digitalWrite(23, HIGH);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    digitalWrite(23, LOW);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void C1S() {
  xTaskCreatePinnedToCore(
    Alarm,
    "Alarm",
    2048,
    NULL,
    5,
    NULL,
    0);
}