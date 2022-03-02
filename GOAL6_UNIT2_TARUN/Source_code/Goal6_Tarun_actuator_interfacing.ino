// TARUN KOTAGIRI GOAL 6 A.K.A UNIT 2 EXERCISE 3.
int send_req();
#include<ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
// Make sure to connect the positive of led(s) to D1 and D2 respectively.
#define LED1 D1
#define LED2 D2

#define MAIN_SSID "Tarun2.4g"  // change this
#define MAIN_PASS "Remastered1!"  //change this

#define CSE_IP      "192.168.1.2" //change this
#define CSE_PORT    8080
#define HTTPS     false
#define OM2M_ORGIN    "admin:admin"
#define OM2M_MN     "/~/mn-cse/mn-name/"
#define OM2M_AE_0     "LAMP_0"
#define OM2M_AE_1     "LAMP_1"
#define OM2M_DATA_CONT  "DATA"
#define LISTENER_PORT  8000

WiFiServer listener(LISTENER_PORT);
WiFiClient client;
HTTPClient http;

bool connect_http();
int post_request(String req);

void setup() {
  // put your setup code here, to run once:
      Serial.begin(9600);
      Serial.println("Connecting to "+ String() + MAIN_SSID);
     WiFi.begin(MAIN_SSID, MAIN_PASS);
  while( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
      Serial.println();
      Serial.println("Tarun Kotagiri UNIT 2 EXERCISE 3 A.K.A GOAL 6");
      Serial.println("COnnection Successful");
      listener.begin(LISTENER_PORT);
      Serial.println("ESP listener started");
      // Serial.println("Ip Address is" + String() + WiFi.localIP());
      delay(500);
  //////////////////////////////Creating a Subscriber content Instance for receiving the data//////////////////////////////////////////////////////

 ///Lamp-0 Object 
  String server="http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;


  http.begin(client, server + String() +OM2M_AE_0 + "/" + OM2M_DATA_CONT + "/");
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=23");
  http.addHeader("Content-Length", "100");
  String req_data = String() + "{\"m2m:sub\": {"
   
  + "\"rn\": \"led_sub_test\""+","


  + "\"nu\": \"" + "http://"+WiFi.localIP().toString()+":"+LISTENER_PORT + "\","

  + "\"nct\": \"2\""

  + "}}";
  int x=http.POST(req_data);
  http.end();

  ////Lamp-1 Object
  http.begin(client, server + String() +OM2M_AE_1 + "/" + OM2M_DATA_CONT + "/");
  http.addHeader("X-M2M-Origin", OM2M_ORGIN);
  http.addHeader("Content-Type", "application/json;ty=23");
  http.addHeader("Content-Length", "100");
  req_data = String() + "{\"m2m:sub\": {"
   
  + "\"rn\": \"led_sub_test\""+","


  + "\"nu\": \"" + "http://"+WiFi.localIP().toString()+":"+LISTENER_PORT + "\","

  + "\"nct\": \"2\""

  + "}}";
  x=http.POST(req_data);
  http.end();
  
 //////Include the sensor setup segments here///////////////////////////////////////////////////////////////////////////////


  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT); 
  Serial.println("ESP Setup Complete");

}

void loop() {
  // put your main code here, to run repeatedly:
  //////////////////////////////////////////////////Receiving data from actuator////////////////////////////////////////////
listener.begin(LISTENER_PORT);
WiFiClient client = listener.available();
String w=client.readString();
Serial.println(w);
int a=w.indexOf("false");
int b=w.indexOf("true");
int c=w.indexOf("LAMP_0");
int d=w.indexOf("LAMP_1");
////////////////////////////////////////////////Turning ON the LEDS based on received data/////////////////////////////////

if(b>0)
{
  if(c>0)
  {
  digitalWrite(LED1,HIGH);
  Serial.println("Light-1 ON");
  }
  else if(d>0)
  {  digitalWrite(LED2,HIGH);
    Serial.println("Light-2 ON");
  }
}
else if(a>0)
{
  if(c>0)
  {
  digitalWrite(LED1,LOW);
  Serial.println("Light-1 OFF");
  }
  else if(d>0)
  {  digitalWrite(LED2,LOW);
    Serial.println("Light-2 OFF");
  }
}


client.flush();

// Send HTTP response to the client
String s = "HTTP/1.1 200 OK\r\n";
client.print(s);
delay(100);

}
