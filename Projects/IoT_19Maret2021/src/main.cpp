#include <Arduino.h>
#include <WiFi.h>

const char* WIFI_SSID = "ArduinoAP";
const char* WIFI_PASS = "Lontongsayur";
const char* HOSTNAME = "BoardESP32IOTSore";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WIFI.mode(WIFI_STA);
  WIFI.begin(WIFI_SSID, WIFI_PASS);
  WIFI.setHostname(HOSTNAME);

  Serial.print("Connecting to WiFi");
  while(Wifi.status() !- WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("WiFi Connected Succsessfully")
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Guntur Nanggala Sakti");
  delay(1000);
}