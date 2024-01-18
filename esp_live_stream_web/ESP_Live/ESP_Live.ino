#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>
#include "DHT.h"

#define DHTPIN 2  
#define DHTTYPE DHT11   // DHT 11
const char* ssid = "Patil's";
const char* password = "1234567890";
DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80);

String page = "";
float data;

void setup() {
  // put your setup code here, to run once:

  pinMode(A0, INPUT);
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.println("");
  dht.begin();  
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to ");
  Serial.println(ssid);
  Serial.println("IP Address : ");
  Serial.println(WiFi.localIP());
  
  
  server.on("/",[](){
    page = "<h1>Sensor to Node MCU Web Server</h1><h2>Current Temperature : " + String(data) + "</h2>",
    server.send(200,"text/html", page);
  });

  server.begin();
  Serial.println("Web Server Started..!");
}

void loop() {
  delay(2000);
  // put your main code here, to run repeatedly:
   float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  data = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(data) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
   // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(data, h, false);
  Serial.print(F("%  Temperature: "));
  Serial.println();
  Serial.print(data);
  
  server.handleClient();
}
