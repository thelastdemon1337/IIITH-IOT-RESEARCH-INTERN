#include "PubSubClient.h"
#include <ESP8266WiFi.h>
#define WaterPin A0

const char* ssid = "Tarun2.4g";
const char* passphrase = "Remastered1!";

char* topic = "channels/1623195/publish/8IOC6I84U3CPACDL";
char* server = "mqtt.thingspeak.com";

WiFiClient wifiClient;
PubSubClient client(server, 1883, wifiClient);

void setup() {
  // put your setup code here, to run once:
  pinMode(WaterPin, INPUT);
  Serial.begin(9600);
  Serial.println();
  Serial.println("Initiating Goal_5 Execution Sequence..");
  Serial.println(F("GOAL_5 : Sending water sensor data to ThingSpeak using MQTT"));
  Serial.println("Root_user : TARUN_KOTAGIRI");
  Serial.println("DATE: 01/01/2022 | TIME: 19:27");
  
  WiFi.begin(ssid, passphrase);
  Serial.print("Connecting to local Access point..");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.println("Connection Success!");
  Serial.print("IPV4 : ");
  Serial.println(WiFi.localIP());

  String clientName = "Water Sensor [ Goal 5 ]";
  Serial.print("Connecting to ");
  Serial.print(server);
  Serial.print(" as ");
  Serial.println(clientName);

  if(client.connect((char*)clientName.c_str())){
    Serial.println("Connected to MQTT Broker");
    Serial.println("Topic is : ");
    Serial.println(topic);

    if(client.publish(topic, "Testing Phase 1 : Communication from ESP8266 to ThingSpeak")){
      Serial.println("Publish Success.");
    } else {
      Serial.println("Publish Failed");
    }
  }
  else{
    Serial.println("MQTT connection failed.");
    abort();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  String payload = "field1=";
  payload += analogRead(WaterPin);
  payload += "&status=MQTTPUBLISH";

  if (client.connected()){
    Serial.print("Sending payload : ");
    Serial.println(payload);

    if(client.publish(topic, (char*) payload.c_str())) {
      Serial.println("Publish OK");
    }
    else {
      Serial.println("Publish Failed.");
    }
  }
  delay(2000);
}
