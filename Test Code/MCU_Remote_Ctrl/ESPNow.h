#include <esp_now.h>
#include <WiFi.h>

#include "esp32-hal-adc.h"

// Addr nya 08:a6:f7:21:aa:94

struct {
  int base, elbow, hook_tarik, hook_ulur, gripper;
} servo_data;

struct {
  float hook_height;
} actuator_data;

uint8_t broadcastAddress[] = { 0x24, 0xDC, 0xC3, 0xC6, 0xBE, 0xDC };

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // if (status == ESP_NOW_SEND_SUCCESS) Serial.println("Data sent!");
  // else Serial.println("Failed to send data!");
}

void IncomingData(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&actuator_data, incomingData, sizeof(actuator_data));
}

void ESPNowS() {
  WiFi.mode(WIFI_STA);
  while (esp_now_init() != ESP_OK) Serial.println("Failed to start ESP-NOW!");

  esp_now_register_recv_cb(IncomingData);
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  while (esp_now_add_peer(&peerInfo) != ESP_OK) Serial.println("Failed to add peer!");
}