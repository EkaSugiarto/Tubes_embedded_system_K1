void Terpaksa(void *pvParameters) {
  while (1) {
    servo_data.base = map(analogRead(34), 0, 4096, 0, 180);
    servo_data.elbow = map(analogRead(35), 0, 4096, 0, 45);
    servo_data.hook_tarik = map(analogRead(32), 0, 4096, 0, 90);
    servo_data.hook_ulur = map(analogRead(33), 0, 4096, 0, 90);


    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&servo_data, sizeof(servo_data));

    Serial.println(map(analogRead(34), 0, 4096, 0, 180));
    Serial.println(map(analogRead(35), 0, 4096, 0, 45));
    Serial.println(map(analogRead(32), 0, 4096, 0, 90));
    Serial.println(map(analogRead(33), 0, 4096, 0, 90));

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void C0S() {
  xTaskCreatePinnedToCore(
    Terpaksa,
    "Terpaksa",
    2048,
    NULL,
    5,
    NULL,
    0);
}