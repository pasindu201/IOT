#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int potPin = A0;

const char* ssid = "Galaxy A20s12d0";
const char* password = "11111111";

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

const char* mqttTopic = "pasinduTopic";
const char* subscribeTopic = "pasinduTopic2"; //For Subscription

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(ssid, password);  //Initialize WiFi Object

  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback); // For Subscription

  while (!client.connected()) {
    Serial.println("Connecting to MQTT..");
    if (client.connect("ClientPasindu")) {
      Serial.print("Connected to MQTT");
      client.subscribe(subscribeTopic); // For Subscription
    } else {
      Serial.println("MQTT Failed to connect");
      delay(5000);
    }
  }
}

void loop() {
  client.loop(); // For Subscription
  // put your main code here, to run repeatedly:
  int potValue = analogRead(A0);
  int mappedPot = map(potValue, 0, 1024, 0, 100);
  //Serial.print("Pot Value : ");
  //Serial.println(mappedPot);

  if (client.connected()) {
    client.publish(mqttTopic, "50");
    //Serial.println("Published to MQTT: hello");
  }

  delay(1000);
}

// For Subscription
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  char payloadStr[length + 1];
  memcpy(payloadStr, payload, length);
  payloadStr[length] = '\0';

  Serial.print("Payload: ");
  Serial.println(payloadStr);
}
