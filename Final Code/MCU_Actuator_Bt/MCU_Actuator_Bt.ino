#include <BluetoothSerial.h>
#include <ESP32Servo.h>

BluetoothSerial SerialBT;

struct {
  int base, elbow, hook, gripper;
} servo_data;

Servo base, elbow, hook, gripper;

SemaphoreHandle_t RunBaseMotor;

const int trigPin = 26;
const int echoPin = 25;
long duration;
float distance;

void parseBTData(String data) {
  if (data.length() != 16) return; // Ensure the data is correctly formatted

  String baseData = data.substring(0, 4);
  String elbowData = data.substring(4, 8);
  String hookData = data.substring(8, 12);
  String gripperData = data.substring(12, 16);

  servo_data.base = (baseData[0] == 'P' ? 1 : -1) * baseData.substring(1).toInt();
  servo_data.elbow = (elbowData[0] == 'P' ? 1 : -1) * elbowData.substring(1).toInt();
  servo_data.hook = (hookData[0] == 'P' ? 1 : -1) * hookData.substring(1).toInt();
  servo_data.gripper = (gripperData[0] == 'P' ? 1 : -1) * gripperData.substring(1).toInt();
}

void BluetoothTask(void *pvParameters) {
  while (1) {
    if (SerialBT.available()) {
      String data = "";
      while (SerialBT.available()) {
        char c = SerialBT.read();
        if (c == '\n') break; // End of message
        data += c;
      }

      parseBTData(data);
      Serial.println("Data received:");
      Serial.println("Base: " + String(servo_data.base));
      Serial.println("Elbow: " + String(servo_data.elbow));
      Serial.println("Hook: " + String(servo_data.hook));
      Serial.println("Gripper: " + String(servo_data.gripper));
    }

    // Send current servo data back via Bluetooth
    String sendData = String(servo_data.base) + ";" + String(servo_data.elbow) + ";" + String(servo_data.hook) + ";" + String(servo_data.gripper) + "\n";
    SerialBT.print(sendData);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void DistanceSensor(void *pvParameters) {
  while (1) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    Serial.println("Distance = " + String(distance));
    if (distance >= 5) xSemaphoreGive(RunBaseMotor);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void BaseServo(void *pvParameters) {
  while (1) {
    base.write(servo_data.base);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void ElbowServo(void *pvParameters) {
  while (1) {
    elbow.write(servo_data.elbow);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void HookServo(void *pvParameters) {
  while (1) {
    hook.write(servo_data.hook);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void GripperServo(void *pvParameters) {
  while (1) {
    gripper.write(servo_data.gripper);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT_Controller"); // Bluetooth name
  Serial.println("Bluetooth started!");

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  base.setPeriodHertz(50);
  base.attach(4);

  elbow.setPeriodHertz(50);
  elbow.attach(16);

  hook.setPeriodHertz(50);
  hook.attach(17);

  gripper.setPeriodHertz(50);
  gripper.attach(5);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  RunBaseMotor = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(
    BluetoothTask,
    "BluetoothTask",
    4096,
    NULL,
    5,
    NULL,
    0);

  xTaskCreatePinnedToCore(
    DistanceSensor,
    "DistanceSensor",
    2048,
    NULL,
    5,
    NULL,
    0);

  xTaskCreatePinnedToCore(
    BaseServo,
    "BaseServo",
    2048,
    NULL,
    5,
    NULL,
    1);

  xTaskCreatePinnedToCore(
    ElbowServo,
    "ElbowServo",
    2048,
    NULL,
    5,
    NULL,
    1);

  xTaskCreatePinnedToCore(
    HookServo,
    "HookServo",
    2048,
    NULL,
    5,
    NULL,
    1);

  xTaskCreatePinnedToCore(
    GripperServo,
    "GripperServo",
    2048,
    NULL,
    5,
    NULL,
    1);

  Serial.println("Setup complete!");
}

void loop() {
  vTaskDelete(NULL); // Prevent loop from running
}
