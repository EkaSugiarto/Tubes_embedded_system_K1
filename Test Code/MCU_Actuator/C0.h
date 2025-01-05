SemaphoreHandle_t RunBaseMotor;

void DistanceSensor(void *pvParameters) {
  while (1) {
    long duration;

    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);

    duration = pulseIn(echoPin1, HIGH);
    float distance = duration * 0.034 / 2;

    if (distance >= 3) xSemaphoreGive(RunBaseMotor);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void C0S() {
  RunBaseMotor = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(
    DistanceSensor,
    "DistanceSensor",
    2048,
    NULL,
    5,
    NULL,
    1);
}