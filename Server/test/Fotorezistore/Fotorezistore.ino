#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "***"; // Enter WIFI ssid
const char *password = "***"; // Enter WIFI password
const char *server_url = "https://nodejsbackendaplus-dfcbc3fc59f6.herokuapp.com/"; // Nodejs application endpoint



StaticJsonDocument<200> jsonDocument;

WiFiClient client;
HTTPClient http;

int ledPin = 2;

void setup() {
  delay(3000);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  delay(1000);
}

void loop() {
  int value = analogRead(A0);

  Serial.print("Analog Value: ");
  Serial.println(value);
  

  if (value > 450) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
  }

  jsonDocument.clear();
  jsonDocument["light"] = value;

  char json_str[100];
  serializeJson(jsonDocument, json_str);

  if (http.begin(client, server_url)) {
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(json_str);
    http.setTimeout(1000);
    if (httpCode > -12) {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.print("Response: ");
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("Unable to connect to server.");
  }


  delay(250);
}
