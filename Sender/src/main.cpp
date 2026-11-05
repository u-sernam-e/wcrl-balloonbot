/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>

#define INX 2
#define INY 3
#define JOYGND 4

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xAC, 0x27, 0x6E, 0x7D, 0x3B, 0x94}; // black
// uint8_t broadcastAddress[] = {0xAC, 0x27, 0x6E, 0x7E, 0x5E, 0xB0}; // pink
// uint8_t broadcastAddress[] = {0xAC, 0x27, 0x6E, 0x7C, 0x9C, 0x78}; // blue
// uint8_t broadcastAddress[] = {0xB0, 0xA6, 0x04, 0x00, 0xD9, 0x64}; // green


// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  float x;
  float y;
} struct_message;

// Create a struct_message called myData
struct_message motorData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  
  const char* success = status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail";
  Serial.print("\tLast Packet Send Status:\t");
  Serial.println(success);
}
 
void setup() {
  // Init Serial Monitor, commenting can fix it not working
  // Serial.begin(115200);

  pinMode(INX, INPUT);
  pinMode(INY, INPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(JOYGND, OUTPUT);
  digitalWrite(JOYGND, LOW);
}

// changing joystick value to readable number,
// my joystick might be faulty so this is to correct it
float adjustJoyValue(int in) {
  float maxValue = 0.5;
  float outputValue;

  if (in < 2300) // the output values have a weird curve so this gives good values
    outputValue = (in - 2300.0) / (2300.0 / maxValue);
  else
    outputValue = (in - 2300.0) / ((4096 - 2300.0) / maxValue);

  // shoudlnt need this but its a failsafe
  if (outputValue > maxValue)
    outputValue = maxValue;
  if (outputValue < -maxValue)
    outputValue = -maxValue;

  // dead zone
  if (abs(outputValue) < 0.05)
    outputValue = 0;
  
  return outputValue;
}
 
void loop() {
  // Set values to send, rotating for the bot orientation
  motorData.x = adjustJoyValue(analogRead(INY));
  motorData.y = -adjustJoyValue(analogRead(INX));
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &motorData, sizeof(motorData));
   
  Serial.print(analogRead(INX));
  Serial.print('\t');
  Serial.print(analogRead(INY));
  Serial.print('\t');
  Serial.print(motorData.x);
  Serial.print('\t');
  Serial.print(motorData.y);
  /*
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }*/
  delay(100);
}

