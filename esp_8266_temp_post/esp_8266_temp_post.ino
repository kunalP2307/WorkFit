#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 2  
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
const char* ssid = "Patil's";
const char* password = "1234567890";
const char* serverName = "http://192.168.240.93:27022/insert-one/env_temp_dht";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
float data;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
unsigned long epochTime; 

void setup() {

    pinMode(A0, INPUT);
    delay(1000);
    Serial.begin(115200);

    Serial.println();
    Serial.println();
    Serial.println();

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
    timeClient.begin();
}

void loop() {
    if ((WiFi.status() == WL_CONNECTED)) {

        WiFiClient client;
        HTTPClient http;

        float temperature = dht.readTemperature();
        Serial.println(temperature);
        if (!isnan(temperature)) {
            epochTime = getTime();
            Serial.print("[HTTP] begin...\n");
            http.begin(client, serverName);  // HTTP
            http.addHeader("Content-Type", "application/json");

            Serial.print("[HTTP] POST...\n");
            
            StaticJsonBuffer<200> jsonBuffer;
            JsonObject& root = jsonBuffer.createObject();

            root["time_stamp"] = epochTime;
            root["temperature"] = temperature;
            
            char jsonChar[100];
            root.printTo((char*)jsonChar,root.measureLength() + 1);
            Serial.println(jsonChar);
            int httpCode = http.POST(jsonChar);
            if (httpCode > 0) {
                Serial.printf("[HTTP] POST... code: %d\n", httpCode);

                if (httpCode == HTTP_CODE_OK) {
                    const String& payload = http.getString();
                    Serial.println("received payload:\n<<");
                    Serial.println(payload);
                    Serial.println(">>");
                }
            } 
            else {
              Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
            http.end();
        }
        else{
          Serial.println("Failed to read temperature!!");
        }
    }
  delay(10000);
}

unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}