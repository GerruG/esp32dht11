# ESP32 IoT Sensor Project

This project utilizes an **ESP32** and a **DHT11** sensor to monitor temperature and humidity. The collected data is transmitted to a Raspberry Pi server via **MQTT** for storage and real-time visualization.

## Code Overview

### Libraries and Definitions

```cpp
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// WiFi credentials
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"

// MQTT broker settings
#define MQTT_HOST "192.168.0.207"   // Replace with your Raspberry Pi IP
#define MQTT_PORT 1883              // Non-SSL port
#define MQTT_TOPIC "/home/sensors"  // MQTT topic for sensor data

// DHT settings
#define DHT_PIN GPIO_NUM_0
#define DHT_TYPE DHTesp::DHT11
```
WiFi.h & WiFiClient.h: Manage WiFi connectivity.
PubSubClient.h: Handles MQTT communication.
DHTesp.h: Interfaces with the DHT11 sensor.
Credentials and Settings: Replace placeholder values with your actual WiFi and MQTT broker details.
MQTT Reconnection Function
cpp
```
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
```
Purpose: Ensures the ESP32 stays connected to the MQTT broker.
Functionality:
Attempts to connect with a unique client ID.
Retries every 5 seconds if the connection fails.
Setup Function
```cpp

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  
  // Initialize DHT sensor
  dht.setup(DHT_PIN, DHT_TYPE);
  
  // Setup MQTT server
  client.setServer(MQTT_HOST, MQTT_PORT);
  
  // Attempt initial MQTT connection
  reconnect();
}
```
Serial Communication: Initializes serial monitor for debugging.
WiFi Connection: Connects to the specified WiFi network.
DHT Sensor Initialization: Sets up the DHT11 sensor on the defined GPIO pin.
MQTT Setup: Configures the MQTT broker details and attempts to connect.
Loop Function
```cpp

void loop() {
  // Ensure MQTT connection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Read temperature and humidity
  TempAndHumidity newValues = dht.getTempAndHumidity();
  
  // Validate sensor readings
  if (isnan(newValues.temperature) || isnan(newValues.humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Format sensor data as JSON
  String payload = "{";
  payload += "\"temperature\":" + String(newValues.temperature, 1) + ",";
  payload += "\"humidity\":" + String(newValues.humidity, 1);
  payload += "}";
  
  // Publish the JSON payload to MQTT topic
  Serial.println("Publishing message: " + payload);
  client.publish(MQTT_TOPIC, payload.c_str());
  
  // Wait for 5 seconds before next reading
  delay(5000);
}
```
MQTT Connection Check: Verifies and maintains the MQTT connection.
Sensor Data Acquisition: Reads temperature and humidity from the DHT11 sensor.
Data Validation: Ensures the sensor readings are valid numbers.
JSON Formatting: Structures the sensor data into a JSON string.
Data Transmission: Publishes the JSON payload to the specified MQTT topic.
Delay: Waits for 5 seconds before taking the next reading.
Complete Code
```cpp

#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// WiFi credentials
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"

// MQTT broker settings
#define MQTT_HOST "192.168.0.207"   // Replace with your Raspberry Pi IP
#define MQTT_PORT 1883              // Non-SSL port
#define MQTT_TOPIC "/home/sensors"  // MQTT topic for sensor data

// DHT settings
#define DHT_PIN GPIO_NUM_0
#define DHT_TYPE DHTesp::DHT11

WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dht;

// Function to handle MQTT reconnection
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
```
void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  
  // Initialize DHT sensor
  dht.setup(DHT_PIN, DHT_TYPE);
  
  // Setup MQTT server
  client.setServer(MQTT_HOST, MQTT_PORT);
  
  // Attempt initial MQTT connection
  reconnect();
}

void loop() {
  // Ensure MQTT connection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Read temperature and humidity
  TempAndHumidity newValues = dht.getTempAndHumidity();
  
  // Validate sensor readings
  if (isnan(newValues.temperature) || isnan(newValues.humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Format sensor data as JSON
  String payload = "{";
  payload += "\"temperature\":" + String(newValues.temperature, 1) + ",";
  payload += "\"humidity\":" + String(newValues.humidity, 1);
  payload += "}";
  
  // Publish the JSON payload to MQTT topic
  Serial.println("Publishing message: " + payload);
  client.publish(MQTT_TOPIC, payload.c_str());
  
  // Wait for 5 seconds before next reading
  delay(5000);
}
