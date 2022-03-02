#include <ESP8266WiFi.h>
WiFiClient client;
#define SSID "Tarun2.4g"
#define PASSPHRASE "@p@$$9raseIs(!supposed)2BEDIScloSED"

#include<ThingSpeak.h>

#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

long ChannelNumber = 1622087;
const char WriteAPIKey[] = "IHDGSX6PC7AZH13H";

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Initiating Goal_2 Execution Sequence..");
  Serial.println(F("GOAL_2 : DHT test!"));
  Serial.println("Root_user : TARUN_KOTAGIRI");
  Serial.println("DATE: 29/12/2021 | TIME: 22:14");
  
  WiFi.begin(SSID, PASSPHRASE);
  Serial.print("Connecting to local Access point..");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.println("Connection Success!");
  Serial.print("IPV4 : ");
  Serial.println(WiFi.localIP());
  dht.begin();
  ThingSpeak.begin(client);
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  float f = dht.readTemperature(true); // returns fahrenheit value;

  if(isnan(h) || isnan(t) || isnan(f)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.println("Temperature: " + (String)t + "°C"); // use 'alt + 0176' for  the degree symbol. 
  Serial.println("Humidity: " + (String)h + "%");
  ThingSpeak.writeField(ChannelNumber, 1, t, WriteAPIKey);
  ThingSpeak.writeField(ChannelNumber, 2, h, WriteAPIKey);
}
