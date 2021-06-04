#include <Arduino.h>
#include <WiFi.h>

const char* WIFI_SSID = "WIFI gratis By ICON+";
const char* WIFI_PASS = "1sampai8";
const char* HOSTNAME = "BoardESP32";
const char* IOTBROKER = "broker.hivemq.com";
#define PIN_RELAY 32

OneWire oneWire(4);
DallasTemperature sensors(&oneWire);
WiFiClient net;
MQTTClient iot;

float getAmbientTemperature()
{
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  float tempC = sensors.getTempCByIndex(0);

  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
    return tempC;
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
    return -127;
  }
}

void setRelay(bool state)
{
  digitalWrite(PIN_RELAY, state);
  Serial.print("Relay state changed to: ");
  Serial.print(state);
  Serial.println();
}

bool getRelay()
{
  bool state = digitalRead(PIN_RELAY);
  Serial.print("Relay state is: ");
  Serial.print(state);
  Serial.println();
  return state;
}

void messageReceived(String &topic, String &payload)
{
  Serial.println("Incomming: " + topic + " - " + payload);

  if(topic == "undiknas/ti/kelompok0/relay")
  {
    if(payload == "on")
    {
      setRelay(1);
    }
    else
    {
      setRelay(0);
    }
  }
}

void iotConnect()
{
  Serial.print("Checking WiFi");
  while(WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");

  Serial.print("Connecting to IoT Broker");
  while(!iot.connect("ESP32", "public", "public")) 
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("IoT broker connected successfully.");
}



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

  sensors.begin();

  iot.begin(IOTBROKER, net);
  iot.onMessage(messageReceived);
  iotConnect();
  iot.subscribe("undiknas/ti/kelompok0/relay");
  iot.subscribe("undiknas/ti/+/chatroom");
}

unsigned long intervalCounterRelay = 0;
unsigned long intervalCounterSensor = 0;
unsigned long intervalCounterIoTConnect = 0;
void loop() {
  unsigned long now = millis();
  // put your main code here, to run repeatedly:
  if((now - intervalCounterIoTConnect) > 10000)
  {
    {
      if(!iot.connected())
      {
        iotConnect(); 
      }
      intervalCounterIoTConnect = now;
    }
  }

  if( (now - intervalCounterRelay) > 1000)
  {
    intervalCounterRelay = now;

    bool relayState = getRelay();
    iot.publish("undiknas/ti/kelompok0/relay/state", String(relayState));
  }

  if( (now - intervalCounterSensor) > 5000)
  {
    intervalCounterSensor = now;

    float suhu = getAmbientTemperature();
    iot.publish("undiknas/ti/kelompok0/sensor/suhu", String(suhu));
  } 
  
  iot.loop();
}
