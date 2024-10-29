#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// WiFi credentials
#define WIFI_SSID "wifi-ssid"
#define WIFI_PASS "wifi-password"

// MQTT broker settings
#define MQTT_HOST "192.168.0.207"   // Replace with your Raspberry Pi IP
#define MQTT_PORT 1883              // Non-SSL port
#define MQTT_TOPIC "/home/sensors"

// DHT settings
#define DHT_PIN GPIO_NUM_0
#define DHT_TYPE DHTesp::DHT11

WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dht;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  dht.setup(DHT_PIN, DHTesp::DHT11);

  client.setServer(MQTT_HOST, MQTT_PORT);
  reconnect();
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  TempAndHumidity newValues = dht.getTempAndHumidity();

  // Validate readings
  if (isnan(newValues.temperature) || isnan(newValues.humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Format the sensor data as JSON
  String payload = "{";
  payload += "\"temperature\":" + String(newValues.temperature, 1) + ",";
  payload += "\"humidity\":" + String(newValues.humidity, 1);
  payload += "}";

  Serial.println("Publishing message: " + payload);
  client.publish(MQTT_TOPIC, payload.c_str());

  delay(5000); // Adjust the delay as needed
}
