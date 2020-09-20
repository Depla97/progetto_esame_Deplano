#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int ledPin[] = {14, 12, 13, 15};
int buttonPin = 5;
int lettura = 1;
int letturaOld = 1;

const char* ssid = "CAPUTOS LAN";
const char* password = "contradauccellierawifi2016";
const char* mqtt_server = "192.168.1.5";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
 
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
}


void callback(char* topic, byte* payload, unsigned int length) {

  String stopic = topic;
  String smessage;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    smessage += (char)payload[i];
  }
  Serial.println();

  messageParser(stopic, smessage);

}

void messageParser(String topic, String message)
{
  if (topic == "led/blue")
  {
    if (message == "ON")
      digitalWrite(ledPin[0], HIGH);
    if (message == "OFF")
      digitalWrite(ledPin[0], LOW);
  }

  if (topic == "led/green")
  {
    if (message == "ON")
      digitalWrite(ledPin[1], HIGH);
    if (message == "OFF")
      digitalWrite(ledPin[1], LOW);
  }

  if (topic == "led/yellow")
  {
    if (message == "ON")
      digitalWrite(ledPin[2], HIGH);
    if (message == "OFF")
      digitalWrite(ledPin[2], LOW);
  }

  if (topic == "led/red")
  {
    if (message == "ON")
      digitalWrite(ledPin[3], HIGH);
    if (message == "OFF")
      digitalWrite(ledPin[3], LOW);
  }

}


void button()
{
  if (lettura == 1 && letturaOld == 0)
  {
    client.publish("button", "premuto");
    letturaOld = 1;
  }

  lettura = digitalRead(buttonPin);

  if (lettura == 0)
  {
    letturaOld = 0;
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      //subscribes to topics for led control
      client.subscribe("led/yellow");
      client.subscribe("led/blue");
      client.subscribe("led/red");
      client.subscribe("led/green");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  for (int i = 0; i < 4; i++)
  {
    pinMode(ledPin[i], OUTPUT);
    Serial.print("Pin ");
    Serial.print(i);
    Serial.println(" output enabled");
  }
  pinMode(buttonPin, INPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  randomSeed(micros());//initializes the RNG for the ClientID
}


void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  button();
}
