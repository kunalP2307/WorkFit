#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "Patil's";
const char* password = "1234567890";
const char* serverName = "http://192.168.240.93:27022/insert-one/env_temp_dht";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {

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
}

void loop() {
    if ((WiFi.status() == WL_CONNECTED)) {

        WiFiClient client;
        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        http.begin(client, serverName);  // HTTP
        http.addHeader("Content-Type", "application/json");

        Serial.print("[HTTP] POST...\n");
        int httpCode = http.POST("{\"key\":\"value\"}");

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
  delay(10000);
}
