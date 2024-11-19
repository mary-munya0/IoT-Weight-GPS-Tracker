# IoT-Weight-GPS-Tracker
This Arduino-based project combines a weight sensor (HX711) and a GPS module to gather real-time weight and location data. The data is processed and sent to AWS IoT Core using the MQTT protocol.
**Key Components**
1. Hardware:
  HX711 weight sensor
  GPS module (TinyGPS++)
  Microcontroller (ESP32)
  
2. Software/Dependencies
  HX711.h: For interfacing with the HX711 weight sensor.
  TinyGPS++.h: For decoding GPS data.
  WiFi.h: For connecting to Wi-Fi.
  PubSubClient.h: For MQTT communication.
  ArduinoJson.h: This is for creating and parsing JSON payloads.

3. Cloud

  AWS IoT Core: Handles MQTT topics and securely processes IoT data.
