#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "AQI";
const char* password = "aqi@123456";
const int port = 8080;

ESP8266WebServer server(port);

DynamicJsonDocument json_data(1024);

void handleRoot() {
  server.send(200, "text/html", "<h1>Hello from ESP8266!</h1>");
}

void handleNotFound(){
  server.send(404, "text/plain", "Not found");
}

void handleData() {
  if (Serial.available() > 0) {
    JsonObject doc = json_data.to<JsonObject>();
    String data = Serial.readStringUntil('\n');
    int index1 = data.indexOf(',');
    int index2 = data.indexOf(',', index1+1);
    int index3 = data.indexOf(',', index2+1);
    float ppm = data.substring(0, index1).toFloat();
    float temperature = data.substring(index1+1, index2).toFloat();
    float humidity = data.substring(index2+1, index3).toFloat();
    float heatIndex = data.substring(index3+1).toFloat();

    doc["co"] = ppm;
    doc["temp"] = temperature;
    doc["humidity"] = humidity;
    doc["heatindex"] = heatIndex;

    // json_array.add(doc);
    String json;
    serializeJson(json_data, json);

    server.send(200, "application/json", json);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress ip(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, ip, subnet);
  delay(100);
  
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.onNotFound(handleNotFound);

  server.begin();
}

void loop() {
  server.handleClient();
}
