#include "HX711.h"
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
 
#define AWS_IOT_PUBLISH_TOPIC   "tracking/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "tracking/sub"
 
// Choose two Arduino pins to use for software serial
int RXPin = 16;
int TXPin = 17;

// HX711 circuit wiring
uint8_t dataPin = 18;
uint8_t clockPin = 19;

int GPSBaud = 9600;

// Create a TinyGPS++ object
TinyGPSPlus gps;
// Create a HX711 object
HX711 scale;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

// this is to hold scale values
volatile float f;
unsigned long prevMillis = 0;  // Variable to store the last execution time
unsigned long delayInterval = 30000;

//setting up variables to store the date and time
volatile float Latitude, Longitude, Altitude;
volatile int year, month, day, hour, minute, second;

//Device ID
const char* DeviceId = "Device_0";
 
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
 
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["DeviceId"] = DeviceId;
  doc["Latitude"] = Latitude;
  doc["Longitude"] = Longitude;
  doc["Altitude"] = Altitude;
  doc["HX711Reading"] = f; 
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  Serial.println("Publishing message...");
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}
 
void setup()
{
 //start the hardware serial
  Serial.begin(115200);

  scale.begin(dataPin, clockPin);
  scale.set_scale(127.15);

  // load cell factor 5 KG
  // scale.set_scale(420.0983);
  // reset the scale to zero = 0
  scale.tare();
  f = 0;


  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
  connectAWS();
  
}

int weight = 0;
void loop()
{
  unsigned long currentTime = millis();
  if (currentTime - prevMillis >= delayInterval) {
    // Call your function here
    weight = getWeight();
    Serial.println(weight);

    // Publish the message
    // publishMessage();

    // Update the last execution time
    prevMillis = currentTime;
  }

  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        Latitude = gps.location.lat();
        Longitude = gps.location.lng();
        Altitude = gps.altitude.meters();

      } else {
        Serial.println("Location: Not Available");
      }
      if (gps.date.isValid()) {
        year = gps.date.year();
        month = gps.date.month();
        day = gps.date.day();
      } else {
        Serial.println("Date Not Valid");
      }
      if (gps.time.isValid()) {
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();
      } else {
        Serial.println("Time Not Valid");
      }

      // displayInfo();
      Serial.print("Latitude: ");
      Serial.print(Latitude,6);
      Serial.print("Longitude: ");
      Serial.print(Longitude,6);
      Serial.print("HX711 Reading: ");
      Serial.print(f);
      Serial.println();
      Serial.println();

      publishMessage();
      client.loop();
      // delay(30000);
      delay(1000);
      // Serial.println(Latitude, 8);
      
    }

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS detected");
    while (true)
      ;
  }
}
int getWeight() {
  if (scale.wait_ready_timeout(1000)) {
    f = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(f);
  } else {
    Serial.println("HX711 not found.");
  }
  return f;
}

void displayInfo() {
  if (gps.location.isValid()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  } else {
    Serial.println("Location: Not Available");
  }

  Serial.print("Date: ");
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  } else {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  } else {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(30000);
}