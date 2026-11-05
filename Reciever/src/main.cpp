
#include <Arduino.h>
#include <WiFi.h>
#include <DShotRMT.h>
#include <esp_now.h>
#include <esp_wifi.h>

#include "main.hpp"

DShotRMT esc(ESC, DSHOT300, false);
float esc_val = 0;

int32_t spin_dir_flag = 0;
bool spin_dir = false;
// DShotRMT esc(ESC);  // oldAPI

void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void mapAndDrive(float x, float y) {
    float angle = (atan2f(y, x) * (180.0f/(float)M_PI));
    angle = (angle < 0.0f) ? angle + 360.0f : angle;
    float mag = hypotf(x, y);

    float vL, vR;
    
    int pwmL, pwmR;
    int dirL, dirR;

    // piecewise for controller mappings; allows easy runtime remapping (not implemented rn tho)
    if (angle >= 0.0f && angle < 90.0f) {
        vL = 1.0f;
        vR = (2.0f/90.0f)*angle - 1.0f; // y = mx+b
    }
    else if (angle >= 90.0f && angle < 180.0f) {
        vL = -(2.0f/90.0f)*angle + 3.0f;
        vR = 1.0f;
    }
    else if (angle >= 180.0f && angle < 270.0f) {
        vL = -1.0f;
        vR = -(2.0f/90.0f)*angle + 5.0f;
    }
    else if (angle >= 270.0f && angle < 360.0f) {
        vL = (2.0f/90.0f)*angle - 7.0f;
        vR = -1.0f;
    }

    // scale by mag
    vL *= mag;
    vR *= mag;

    pwmL = int(abs(vL) * 255.0f);
    pwmR = int(abs(vR) * 255.0f);

    dirL = (vL < 0.0f) ? -1 : 1;
    dirR = (vR < 0.0f) ? -1 : 1;

    switch(dirL) {
        case(-1):
            digitalWrite(AIN1, LOW);
            digitalWrite(AIN2, HIGH);
            break;
        case(1):
            digitalWrite(AIN1, HIGH);
            digitalWrite(AIN2, LOW);
            break;
    }
    switch(dirR) {
        case(-1):
            digitalWrite(BIN1, LOW);
            digitalWrite(BIN2, HIGH);
            break;
        case(1):
            digitalWrite(BIN1, HIGH);
            digitalWrite(BIN2, LOW);
            break;
    }

    analogWrite(PWMA, pwmL);
    analogWrite(PWMB, pwmR);

    Serial.print("L: ");
    Serial.print(pwmL);
    Serial.print(", ");
    Serial.print(dirL);
    Serial.print(", R: ");
    Serial.print(pwmR);
    Serial.print(", ");
    Serial.println(dirR);
}

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  float x;
  float y;
} struct_message;

// Create a struct_message called myData
struct_message motorData;
// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&motorData, incomingData, sizeof(motorData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("X: ");
  Serial.println(motorData.x);
  Serial.print("Y: ");
  Serial.println(motorData.y);
  Serial.println();

  mapAndDrive(motorData.x, motorData.y);

}

void setup() {
    delay(250);
    Serial.begin(115200);
    pinMode(STAT, OUTPUT);
    pinMode(PWMA, OUTPUT);  // pulldown for safety?
    pinMode(PWMB, OUTPUT);  // pulldown for safety? 
    pinMode(AIN1, OUTPUT); 
    pinMode(AIN2, OUTPUT); 
    pinMode(BIN1, OUTPUT); 
    pinMode(BIN2, OUTPUT); 
    pinMode(ESC, OUTPUT);   // pulldown for safety?
    
    esp_reset_reason_t reset_reason = esp_reset_reason();
    if (reset_reason == ESP_RST_BROWNOUT) {
        digitalWrite(STAT, HIGH);
        delay(250);
        digitalWrite(STAT, LOW);
        delay(250);
        digitalWrite(STAT, HIGH);
        delay(250);
        digitalWrite(STAT, LOW);
        delay(250);
    }
    // esc
    esc.begin();
    // printCpuInfo(Serial);

    // Initialize AP Mode
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    
    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

unsigned pmillis = 0;
void loop() {/*
    httpServer.handleClient();
    webSocket.loop();
    if (millis() - pmillis >= 2) {
        pmillis = millis();
        
        if (spin_dir_flag > 0) {
            esc.setMotorSpinDirection(spin_dir);
            spin_dir_flag--;
            Serial.print("dir flipped?: ");
            Serial.println(spin_dir);
            delay(2);
            // esc.sendCommand(DSHOT_CMD_BEACON1);
            delay(400);
        }
        else {
            esc.sendThrottle(esc_val);
            // Serial.println(esc_val);
        }
    }
*/
}
