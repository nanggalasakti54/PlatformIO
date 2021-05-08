#include <Arduino.h>
#include <WiFi.h>

const char* WIFI_SSID = "WIFI gratis By ICON+";
const char* WIFI_PASS = "1sampai8";
const char* HOSTNAME = "BoardESP32";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setHostname(HOSTNAME);

  Serial.print("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("WiFi Connected Succsessfully");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(115200);

}