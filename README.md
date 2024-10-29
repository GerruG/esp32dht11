ESP32 IoT Sensor Project
This project uses an ESP32 and DHT11 sensor to collect temperature and humidity data, which is then transmitted to a Raspberry Pi server over MQTT for storage and visualization.

Features
Temperature and Humidity Monitoring: Gathers real-time data from a DHT11 sensor.
Data Transmission via MQTT: Sends JSON-formatted sensor data to an MQTT broker.
ESP-IDF with PlatformIO: Developed and flashed using ESP-IDF in PlatformIO.
Real-Time Data Visualization: Data is processed and visualized on the server.
Setup Instructions
Clone the Repository:

bash
Kopiera kod
git clone https://github.com/your-username/your-repo.git
Configure WiFi and MQTT Credentials:

Open the code and replace placeholders with your actual WiFi and MQTT details:
cpp
Kopiera kod
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"
#define MQTT_HOST "your_mqtt_host"
Flash the ESP32:

Connect the ESP32 to your computer and use PlatformIO to upload the code.
Run the System:

Ensure the Raspberry Pi server is running the MQTT broker.
The ESP32 will automatically connect and begin transmitting sensor data.
Requirements
Hardware: ESP32, DHT11 sensor
Software: PlatformIO, ESP-IDF
Notes
Ensure MQTT broker settings match your Raspberry Pi server configuration.
Placeholder values for WiFi and MQTT credentials are used for security; replace them locally.
