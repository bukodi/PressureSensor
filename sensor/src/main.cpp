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

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_MODE_STA);
    Serial.println("sensor");
    Serial.println(WiFi.macAddress());

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }


    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    // Register peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, DISPLAY_MAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
    myMessage.adcBattery = 4096;
}

void loop() {
    // Set values to send
    myMessage.adcPressure = random(10, 3072);
    myMessage.adcBattery--;

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(DISPLAY_MAC, (uint8_t *)&myMessage, sizeof(myMessage));

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    }
    else {
        Serial.println("Error sending the data");
    }
    delay(2000);
}