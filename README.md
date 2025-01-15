IoT Weight and Location Tracker

This project integrates a weight sensor (HX711) and a GPS module to gather real-time weight and location data. The processed data is sent securely to AWS IoT Core using the MQTT protocol.

Key Features

Real-time weight measurement using the HX711 sensor.

GPS tracking for location and altitude data.

Secure communication with AWS IoT Core using TLS encryption.

Data transmission in JSON format over MQTT.


Components

Hardware

HX711 Weight Sensor: Measures weight data.

GPS Module (TinyGPS++): Tracks location, altitude, date, and time.

Microcontroller (ESP32): Serves as the central processing unit for the module.


Software/Dependencies

Ensure the following libraries are installed in your Arduino IDE:

HX711.h: For interfacing with the HX711 weight sensor.

TinyGPS++.h: For decoding GPS data.

WiFi.h: For establishing Wi-Fi connectivity.

PubSubClient.h: For MQTT communication.

ArduinoJson.h: For creating and parsing JSON payloads.


Cloud Integration

AWS IoT Core:

Handles MQTT topics (tracking/pub and tracking/sub).

Processes IoT data securely with certificates and keys.


Setup Instructions

Hardware Connections

1. HX711 Weight Sensor:

Data pin to GPIO 18.

Clock pin to GPIO 19.



2. GPS Module:

RX pin to GPIO 16.

TX pin to GPIO 17.



3. Power the components via the ESP32's 3.3V and GND pins.



Software Setup

1. Clone this repository:

git clone <link-to-this-repo>


2. Open the code in the Arduino IDE.


3. Install the required libraries:

In Arduino IDE → Sketch → Include Library → Manage Libraries → Search and Install.



4. Update the following placeholders in the secrets.h file:

Wi-Fi credentials: WIFI_SSID and WIFI_PASSWORD.

AWS IoT credentials: AWS_CERT_CA, AWS_CERT_CRT, AWS_CERT_PRIVATE, and AWS_IOT_ENDPOINT.


Running the Module

1. Connect the hardware components as specified.


2. Upload the code to the ESP32 using Arduino IDE.


3. Open the Serial Monitor (baud rate: 115200) to view logs.


4. Verify successful connections:

Wi-Fi: Logs indicate "Wi-Fi Connected".

AWS IoT Core: Logs indicate "AWS IoT Connected".


Expected Output

1. Serial Monitor Logs:

Weight data from the HX711 sensor.

GPS data: Latitude, Longitude, Altitude, Date, and Time.



2. AWS IoT Core:

Published messages appear on the tracking/pub topic in JSON format, e.g.:

{  
  "DeviceId": "Device_0",  
  "Latitude": -1.2921,  
  "Longitude": 36.8219,  
  "Altitude": 1661.0,  
  "HX711Reading": 5.23  
}


Troubleshooting

1. Wi-Fi Connection Issues:

Double-check WIFI_SSID and WIFI_PASSWORD in the secrets.h file.

Ensure the ESP32 is within range of the Wi-Fi router.


2. AWS IoT Connection Errors:

Verify the AWS IoT certificates and endpoint in the secrets.h file.

Confirm the MQTT topic configuration on AWS IoT Core.


3. GPS Not Detecting Location:

Ensure the GPS module is placed in an open area with a clear view of the sky.

Verify RX and TX pin connections.


4. HX711 Not Responding:

Confirm proper wiring between the HX711 and ESP32.

Ensure the scale calibration factor is appropriate for your load cell.



Future Enhancements

Add support for additional sensors or data processing features.

Implement a local fallback option for data storage.

Integrate with additional cloud services like DynamoDB for data storage or Lambda for event-based processing.


