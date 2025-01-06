#include <esp_now.h>
#include <WiFi.h>

// MAC addr 24:dc:c3:c6:be:dc

struct {
  int base, elbow, hook_tarik, hook_ulur, gripper;
} servo_data;

struct {
  float hook_height;
} actuator_data;

uint8_t broadcastAddress[] = { 0x08, 0xA6, 0xF7, 0x21, 0xAA, 0x94 };

esp_now_peer_info_t peerInfo;

void IncomingData(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&servo_data, incomingData, sizeof(servo_data));
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // if (status == ESP_NOW_SEND_SUCCESS) Serial.println("Data sent!");
  // else Serial.println("Failed to send data!");
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