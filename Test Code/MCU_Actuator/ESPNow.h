#include <esp_now.h>
#include <WiFi.h>

// MAC addr ESP ini 24:dc:c3:c6:be:dc

// Struct yang berisi data remote control yang dikirim ke aktuator
struct {
  int base, elbow, hook_tarik, hook_ulur, gripper;
} servo_data;

// Struct yang berisi data dari aktuator yang diterima oleh remote control
struct {
  float hook_height;
} actuator_data;

// MAC address remote control
uint8_t broadcastAddress[] = { 0x08, 0xA6, 0xF7, 0x21, 0xAA, 0x94 };

// Fungsi untuk menyimpan info peer dari ESP-NOW (ESP remote control)
esp_now_peer_info_t peerInfo;

// Fungsi yang dijalankan ketika data sudah dikirimkan, untuk mengecek status apakah data terkirim atau tidak ke peer (ESP remote control)
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // if (status == ESP_NOW_SEND_SUCCESS) Serial.println("Data sent!");
  // else Serial.println("Failed to send data!");
}

// Fungsi yang dijalankan ketika data masuk dari peer (ESP remote control) ke ESP remote control
void IncomingData(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&servo_data, incomingData, sizeof(servo_data));
}

// Fungsi setup ESP-NOW
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