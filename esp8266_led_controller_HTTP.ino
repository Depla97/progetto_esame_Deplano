                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
#include <ESP8266WiFi.h>
const char WiFiPassword[] = "wifipassword";
const char AP_Name[] = "ESP_WiFi" ;

int ledPin[] = {14, 12, 13, 15};
String comandi[4];

WiFiServer server(80);

String request = "";

void setup()
{
  Serial.begin(115200);
  Serial.println();
  
  for (int i = 0; i < 4; i++)
  {
    pinMode(ledPin[i], OUTPUT);
    Serial.print("Pin ");
    Serial.print(i);
    Serial.println(" output enabled");
  }

  boolean conn = WiFi.softAP(AP_Name, WiFiPassword);
  server.begin();

}

void loop()
{
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  request = client.readStringUntil('\r');
  Serial.print("la richiesta arrivata e': ");
  Serial.println(request);
  requestParser(request, '/', 0, comandi);
  
  client.print(comandi[1]);
  client.print(" ");
  client.print(comandi[2]);
  client.print(" ");
  client.println(comandi[3]);

  ledController(comandi[2], comandi[3]);
}

void requestParser(String request, char delimiter, int iter, String* parsed)
{

  //GET /led/color/status HTTP/1.1
  int index = 0;
  int lunghezza = request.length();

  if (iter != 3)
  {
    index = delimiterFinder(request, delimiter);
    if (index != 0)
    {
      parsed[iter] = request.substring(0, index);
      Serial.println(parsed[iter]);
      request = request.substring(index + 1, lunghezza - 1);
      Serial.println(iter);
      requestParser(request, delimiter, ++iter, parsed);
    }
  }
  if (iter == 3)
  {
    index = delimiterFinder(request, ' ');
    parsed[iter] = request.substring(0, index);
    Serial.println(parsed[iter]);
    return;
  }


}

int delimiterFinder(String line, char delimiter)
{
  int lunghezza = line.length();

  for (int i = 0; i < lunghezza; i++)
  {
    if (line.charAt(i) == delimiter)
      return i;
  }
  return 0;
}

void ledController(String color, String command)
{
  if (color == "blue")
  {
    if (command == "ON" || command == "on")
      digitalWrite(ledPin[0], HIGH);
    if (command == "OFF" || command == "off")
      digitalWrite(ledPin[0], LOW);
  }

  if (color == "green")
  {
    if (command == "ON" || command == "on")
      digitalWrite(ledPin[1], HIGH);
    if (command == "OFF" || command == "off")
      digitalWrite(ledPin[1], LOW);
  }

  if (color == "yellow")
  {
    if (command == "ON" || command == "on")
      digitalWrite(ledPin[2], HIGH);
    if (command == "OFF" || command == "off")
      digitalWrite(ledPin[2], LOW);
  }

  if (color == "red")
  {
    if (command == "ON" || command == "on")
      digitalWrite(ledPin[3], HIGH);
    if (command == "OFF" || command == "off")
      digitalWrite(ledPin[3], LOW);
  }

}
