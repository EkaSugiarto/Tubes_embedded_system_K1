#include <esp_now.h>
#include <WiFi.h>

void Data_masuk(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&servo_data, incomingData, sizeof(servo_data));
}

void ESPNowS() {
  WiFi.mode(WIFI_STA);
  while (esp_now_init() != ESP_OK) Serial.println("Gagal memulai ESP-NOW");

  esp_now_register_recv_cb(Data_masuk);
}