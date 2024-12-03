#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ESP32Servo.h>

BluetoothSerial SerialBT;

#define BASE_SERVO_PIN 15
#define ELBOW_SERVO_PIN 16
#define HOOK_SERVO_PIN 17
#define GRIPPER_SERVO_PIN 18

Servo baseServo, elbowServo, hookServo, gripperServo;

volatile int baseAngle = 90, elbowAngle = 90, hookAngle = 90, gripperAngle = 90;

SemaphoreHandle_t baseSemaphore, elbowSemaphore, hookSemaphore, gripperSemaphore;

void btTask(void *param);
void servoBaseTask(void *param);
void servoElbowTask(void *param);
void servoHookTask(void *param);
void servoGripperTask(void *param);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Crane");
  Serial.println("Bluetooth started. Waiting for commands...");

  baseServo.attach(BASE_SERVO_PIN);
  elbowServo.attach(ELBOW_SERVO_PIN);
  hookServo.attach(HOOK_SERVO_PIN);
  gripperServo.attach(GRIPPER_SERVO_PIN);

  baseSemaphore = xSemaphoreCreateBinary();
  elbowSemaphore = xSemaphoreCreateBinary();
  hookSemaphore = xSemaphoreCreateBinary();
  gripperSemaphore = xSemaphoreCreateBinary();

  baseServo.write(baseAngle);
  elbowServo.write(elbowAngle);
  hookServo.write(hookAngle);
  gripperServo.write(gripperAngle);

  xTaskCreatePinnedToCore(btTask, "Bluetooth Task", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(servoBaseTask, "Base Servo Task", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(servoElbowTask, "Elbow Servo Task", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(servoHookTask, "Hook Servo Task", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(servoGripperTask, "Gripper Servo Task", 2048, NULL, 1, NULL, 1);
}

void loop() {
}

void btTask(void *param) {
  String command;
  while (true) {
    if (SerialBT.available()) {
      command = SerialBT.readStringUntil('\n');
      if (command.length() == 4) {
        baseAngle = 90 + (command[0] - '0');
        elbowAngle = 90 + (command[1] - '0');
        hookAngle = 90 + (command[2] - '0');
        gripperAngle = 90 + (command[3] - '0');

        xSemaphoreGive(baseSemaphore);
        xSemaphoreGive(elbowSemaphore);
        xSemaphoreGive(hookSemaphore);
        xSemaphoreGive(gripperSemaphore);
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void servoBaseTask(void *param) {
  while (true) {
    if (xSemaphoreTake(baseSemaphore, portMAX_DELAY)) {
      baseServo.write(baseAngle);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void servoElbowTask(void *param) {
  while (true) {
    if (xSemaphoreTake(elbowSemaphore, portMAX_DELAY)) {
      elbowServo.write(elbowAngle);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void servoHookTask(void *param) {
  while (true) {
    if (xSemaphoreTake(hookSemaphore, portMAX_DELAY)) {
      hookServo.write(hookAngle);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void servoGripperTask(void *param) {
  while (true) {
    if (xSemaphoreTake(gripperSemaphore, portMAX_DELAY)) {
      gripperServo.write(gripperAngle);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}