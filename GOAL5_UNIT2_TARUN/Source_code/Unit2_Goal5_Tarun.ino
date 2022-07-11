// TARUN KOTAGIRI GOAL 5 UNIT 2
// STEP 1:
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
// WiFiClientSecure httpsClient;

int send_req();

#include<ESP8266HTTPClient.h>
WiFiClient client;
HTTPClient http;
// http.begin(client, serverNameon);

// Step 2:
#define MAIN_SSID "Tarun2.4g"
#define MAIN_PASS "P@$$phraseis!supposed2be->closed!"
#define CSE_IP      "192.168.1.2" //change this
#define CSE_PORT    8080
// #define HTTPS     false
#define OM2M_ORGIN    "admin:admin"
#define OM2M_IN     "/~/in-cse/in-name/"
#define OM2M_AE     "Farmlands"
#define OM2M_DATA_CONT  "DATA"

// Sensors
#include "DHT.h"
#define DHTPIN D1
#define DHTTYPE DHT11
#define SOILPIN A0 // comment this out

DHT dht(DHTPIN, DHTTYPE);


// Step 3:

bool connect_http();
 int post_request(String req);




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    Serial.println("Initiating Network Connection.");
  WiFi.begin(MAIN_SSID,MAIN_PASS);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Connected successfully");
    dht.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  float soil_moist = analogRead(SOILPIN);  // comment out
  float temp = dht.readTemperature();
  float rh = dht.readHumidity();

  String data = "[" + String(temp) + "," + String(rh) + "," + String(soil_moist) + "]";
  String Server = "http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_IN;
  String URL= Server + String() +OM2M_AE + "/" + OM2M_DATA_CONT + "/";
  http.begin(client, URL);
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");

  String req_data = String() + "{\"m2m:cin\": {"

    + "\"con\": \"" + data + "\","

    + "\"cnf\": \"text\""

    + "}}";
  int code = http.POST(req_data);
  http.end();
  Serial.println(code);
  delay(2000);
}
