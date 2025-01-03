#include <esp_now.h>
#include <WiFi.h>

#include "esp32-hal-adc.h"
struct {
  int base, elbow, hook_tarik, hook_ulur;
} servo_data;

uint8_t broadcastAddress[] = { 0x24, 0xDC, 0xC3, 0xC6, 0xBE, 0xDC };

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) Serial.println("Data terkirim ke MCU Act!");
  else Serial.println("Data gagal dikirim!");
}

void ESPNowS() {
  WiFi.mode(WIFI_STA);
  while (esp_now_init() != ESP_OK) Serial.println("Gagal memulai ESP-NOW");
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  while (esp_now_add_peer(&peerInfo) != ESP_OK) Serial.println("Gagal menambahkan peer");
}