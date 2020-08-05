#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include "Setup25_TTGO_T_Display.h"    // Setup file for ESP32 and TTGO T-Display ST7789V SPI bus TFT
#define USER_SETUP_LOADED
#include <TFT_eSPI.h>
#include <SPI.h>

const uint8_t SENSOR_MAC[] ={ 0x3C, 0x71, 0xBF, 0xEE, 0x52, 0xAC };
const uint8_t DISPLAY_MAC[] ={ 0x24, 0x6F, 0x28, 0x96, 0x7C, 0x50 };

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int adcPressure;
    int adcBattery;
} struct_message;

// Create a struct_message called myData
struct_message myMessage;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myMessage, incomingData, sizeof(myMessage));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("ADC pressure: ");
  Serial.println(myMessage.adcPressure);
  Serial.print("ADC battery: ");
  Serial.println(myMessage.adcBattery);
  Serial.println();
}

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println("display");
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  
}

void loop() {
  // put your main code here, to run repeatedly:
}