// Semaphore untuk menyalakan task pada fungsi RTOS BaseServo() di Core 1
SemaphoreHandle_t RunBaseMotor;

// Variabel untuk menyimpan value hasil pengukuran sensor ultrasonic
long duration;
float distance;

// Fungsi RTOS untuk membaca ketinggian dari gripper
void DistanceSensor(void *pvParameters) {
  while (1) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    actuator_data.hook_height = distance;

    Serial.println("Distance = " + String(distance));
    if (distance >= 5) xSemaphoreGive(RunBaseMotor);

    // Mengirim informasi ketinggian gripper ke ESP remote control
    esp_now_send(broadcastAddress, (uint8_t *)&actuator_data, sizeof(actuator_data));

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Setup fungsi RTOS yang dijalankan di Core 0 ESP32
void C0S() {
  RunBaseMotor = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(
    DistanceSensor,
    "DistanceSensor",
    2048,
    NULL,
    5,
    NULL,
    0);
}