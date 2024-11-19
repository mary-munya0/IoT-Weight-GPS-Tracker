#include <pgmspace.h>

#define SECRET

// Define a unique identifier for your device
#define THINGNAME "Tracking"

// Wi-Fi credentials (update with your network details)
const char WIFI_SSID[] = "Your-WiFi-SSID"; // Replace with your Wi-Fi SSID
const char WIFI_PASSWORD[] = "Your-WiFi-Password"; // Replace with your Wi-Fi password

// AWS IoT endpoint (update with your endpoint from the AWS IoT console)
const char AWS_IOT_ENDPOINT[] = "your-aws-endpoint.iot.<region>.amazonaws.com"; // Replace with your AWS IoT endpoint

// Amazon Root CA 1 (update with the root CA provided by AWS IoT)
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
<Insert Amazon Root CA 1 here>
-----END CERTIFICATE-----
)EOF";

// Device Certificate (update with your device certificate)
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
<Insert your device certificate here>
-----END CERTIFICATE-----
)KEY";

// Device Private Key (update with your private key)
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
<Insert your private key here>
-----END RSA PRIVATE KEY-----
)KEY";
