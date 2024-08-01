#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

// Update these with values suitable for your network.
const char* ssid = "Zylog";
const char* password = "Ares123!@#";
const char* mqtt_server = "iotsmarthouse.go.ro";
const char* mqtt_user = "baiaMare";
const char* mqtt_password = "Ares";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (200)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void parsePayload(char* payload, unsigned int length, float& x, float& y) {
  // Create a buffer to hold the payload string
  char payloadStr[length + 1];
  strncpy(payloadStr, (char*)payload, length);
  payloadStr[length] = '\0';

  // Find the positions of the x and y values
  char* xPtr = strstr(payloadStr, "X:");
  char* yPtr = strstr(payloadStr, "Y:");

  if (xPtr != nullptr && yPtr != nullptr) {
    // Parse the x and y values
    x = atof(xPtr + 2);
    y = atof(yPtr + 2);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Variables to hold the parsed x and y coordinatesa
  float x = 0.0, y = 0.0;

  // Parse the payload to extract x and y coordinates
  parsePayload((char*)payload, length, x, y);

  // Print the parsed coordinates
  Serial.print("Parsed x: ");
  Serial.println(x);
  Serial.print("Parsed y: ");
  Serial.println(y);
  Wire.begin();

  // Send a message over I2C based on the value of x
  Wire.beginTransmission(8); // I2C address of the device
  if (x >= 0.5 && x <= 1) {
    Wire.write(10); // Send message 10
  }if (x >= 1) {
    Wire.write(11); // Send message 10
  }
  if (x <= 0.5 && x >= 0) {
    Wire.write(12); // Send message 10
  }else {
    Wire.write(23); // Send message 23
  }
  Wire.endTransmission();
  Wire.endTransmission();

  // Switch on the LED if x is greater than 0 (example condition)
  if (x > 0) {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (active low)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
