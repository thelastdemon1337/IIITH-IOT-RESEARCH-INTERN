#include "WiFi.h"
#include <WiFiClientSecure.h>
char* ssid = "Tarun2.4g";
char* password =  "Remastered1!";
void wifi_connect(char[], char[]);


int send_req();


#include<HTTPClient.h>
WiFiClient client;
HTTPClient http;


#define CSE_IP      "192.168.1.2" //change this
#define CSE_PORT    8080
// #define HTTPS     false
#define OM2M_ORGIN    "admin:admin"
#define OM2M_IN     "/~/in-cse/in-name/"
#define OM2M_AE     "Smart_Indoor_Farm"
#define OM2M_DHT_CONT  "DATA/DHT11"
#define OM2M_WATER_CONT  "DATA/Water-Level"
#define OM2M_SOIL_CONT  "DATA/Soil-Moisture"


#include "DHT.h"
#define DHTPIN 5
#define water_depth_sensor 36
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define SOILPIN 39


bool connect_http();
int post_request(String req);


String Server = "http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_IN;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  wifi_connect(ssid, password);
  pinMode(water_depth_sensor, INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(SOILPIN, INPUT);

  
  dht.begin();
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  int water_level = analogRead(water_depth_sensor);
  Serial.print("Water Level: ");
  Serial.println(water_level);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();   // Read temperature as Celsius (the default)
  float soil_moist = analogRead(SOILPIN);
  // float water_depth = ;
  if(isnan(h) || isnan(t)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.println("Temperature: " + (String)t + "°C"); // use 'alt + 0176' for  the degree symbol. 
  Serial.println("Humidity: " + (String)h + "%");
  Serial.println("Soil Moisture: " + (String)soil_moist);


  // Need optimization: start
  String dht_data = "[" + String(t) + "," + String(h) + "]";
    int str_len = dht_data.length() + 1;
  char dht_array[str_len];
  dht_data.toCharArray(dht_array, str_len);
  String water_data = "[" + String(water_level) + "]";
    str_len = water_data.length() + 1;
  char water_array[str_len];
  water_data.toCharArray(water_array, str_len);
  String soil_data = "[" + String(soil_moist) + "]";
   str_len = soil_data.length() + 1;
  char soil_array[str_len];
  soil_data.toCharArray(soil_array, str_len);
  
  // End

  Serial.println(om2m_post(OM2M_DHT_CONT, dht_array));

  // WATER DATA
  Serial.println(om2m_post(OM2M_WATER_CONT, water_array));

  // SOIL DATA

  
  Serial.println(om2m_post(OM2M_SOIL_CONT, soil_array));


  delay(2000);
}



// CUSTOM FUNCTIONS
void wifi_connect(char ssid[], char pass[]){
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to local Access point..");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.println("Connection Success!");
  Serial.print("IPV4 : ");
  Serial.println(WiFi.localIP());
}

int om2m_post(char parameter[],char data[]){
  String url = Server + String() +OM2M_AE + "/" + parameter + "/";
  http.begin(client, url);
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=4");
  http.addHeader("Content-Length", "100");

  String req_data = String() + "{\"m2m:cin\": {"

    + "\"con\": \"" + data + "\","

    + "\"cnf\": \"text\""

    + "}}";
  int code = http.POST(req_data);
  http.end();
  return code;
}