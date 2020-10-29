#include "FS.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>// allows you to send and receive mqtt messages
#include <NTPClient.h> // include time information from the internet
#include <WiFiUdp.h>
#define NTP_OFFSET   60 * 60      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "in.pool.ntp.org"
int Gas_analog = A0;    // used for ESP8266
int Gas_digital = 1;   // used for ESP8266

// Update these with values suitable for your network.

const char* ssid = "dg1";
const char* password = "Ashi@98563";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);


const char* AWS_endpoint = "arufwoersqlu4-ats.iot.us-east-2.amazonaws.com"; //MQTT broker ip

void callback(char* topic, byte* payload, unsigned int length) {
Serial.print("Message arrived [");
Serial.print(topic);
Serial.print("] ");
for (int i = 0; i < length; i++) {
Serial.print((char)payload[i]);
}
Serial.println();

}
WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callback, espClient); //set MQTT port number to 8883 as per //standard
#define BUFFER_LEN 256
char msg[BUFFER_LEN];
long lastMsg = 0;

int value = 0;
byte mac[6];
char mac_Id[18];
void setup_wifi() {

delay(10);
// We start by connecting to a WiFi network
espClient.setBufferSizes(512, 512);
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

timeClient.begin();
while(!timeClient.update()){
timeClient.forceUpdate();
}

espClient.setX509Time(timeClient.getEpochTime());

}

void reconnect() {
// Loop until we're reconnected
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
// Attempt to connect
if (client.connect("ESPthing")) {
Serial.println("connected");
// Once connected, publish an announcement...
client.publish("outTopic", "hello");
// ... and resubscribe
client.subscribe("inTopic");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");

char buf[256];
espClient.getLastSSLError(buf,256);
Serial.print("WiFiClientSecure SSL error: ");
Serial.println(buf);

// Wait 5 seconds before retrying
delay(5000);
}
}
}

void setup() {
  Serial.begin(115200);  
  pinMode(Gas_digital, INPUT);
  Serial.setDebugOutput(true);
     
timeClient.begin();
setup_wifi();
delay(1000);
if (!SPIFFS.begin()) {
Serial.println("Failed to mount file system");
return;
}

Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());

// Load certificate file
File cert = SPIFFS.open("/cert.der", "r"); 
if (!cert) {
Serial.println("Failed to open cert file");
}
else
Serial.println("Success to open cert file");

delay(1000);

if (espClient.loadCertificate(cert))
Serial.println("cert loaded");
else
Serial.println("cert not loaded");

// Load private key file
File private_key = SPIFFS.open("/private.der", "r"); 
if (!private_key) {
Serial.println("Failed to open private cert file");
}
else
Serial.println("Success to open private cert file");

delay(1000);

if (espClient.loadPrivateKey(private_key))
Serial.println("private key loaded");
else
Serial.println("private key not loaded");

// Load CA file
File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
if (!ca) {
Serial.println("Failed to open ca ");
}
else
Serial.println("Success to open ca");

delay(1000);

if(espClient.loadCACert(ca))
Serial.println("ca loaded");
else
Serial.println("ca failed");

Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());

//===========================================================================
WiFi.macAddress(mac);
snprintf(mac_Id, sizeof(mac_Id), "%02x:%02x:%02x:%02x:%02x:%02x",
mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
Serial.print(mac_Id);
//============================================================================

}

int AirQualityIndex = analogRead(Gas_analog);
int gassensorDigital = digitalRead(Gas_digital);

//int gassensorAnalog = analogRead(Gas_analog);
//int gassensorDigital = digitalRead(Gas_digital);

void loop() {

if (!client.connected()) {
reconnect();
}
client.loop();
timeClient.update();
long now = millis();
if (now - lastMsg > 2000) {
lastMsg = now;
++value;


String formattedTime = timeClient.getFormattedTime();

//snprintf (msg, BUFFER_LEN, "{ "time":'formattedTime',"AQI":'String(AirQualityIndex)'}",  String(AirQualityIndex).c_str(),formattedTime.c_str());

  Serial.print("Gas Sensor: ");
  Serial.print(AirQualityIndex);
  Serial.print("\t");
  Serial.print("Gas Class: ");
  Serial.print(gassensorDigital);
  Serial.print("\t");
  Serial.print("\t");
  
  if (AirQualityIndex > 1000) {
    Serial.println("Gas");
  }
  else {
    Serial.println("No Gas");
  }
  delay(100);
}

client.publish("outTopic", msg);
Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());   //Low heap can cause problems
}
