#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

SoftwareSerial mySerial2(3,2);
/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "iec2016057"               // Adafruit IO Username 
#define AIO_KEY         "89e9fded9a6449a5a82acaaa791790c5" // Adafruit IO Key

int sensor[5]; 
int i=0;

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish vib = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/vibration");
Adafruit_MQTT_Publish temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish current = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/current");
Adafruit_MQTT_Publish vol = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/voltage");
Adafruit_MQTT_Publish rpm= Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/speed");

// Setup a feed called 'Sensor_lock' for subscribing to changes.
//Adafruit_MQTT_Subscribe Sensor_lock = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Sensor_lock");

/*************************** Sketch Code ************************************/

void MQTT_connect();

void setup() {
  Serial.begin(9600);
  mySerial2.begin(9600);
  pinMode(3,INPUT);
  pinMode(2,OUTPUT);
  //delay(100);

  Serial.println();
  Serial.print("Connecting to ");


  WiFi.begin("Test","aaaaaaaa");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

uint32_t x=0;

 
void loop() {

if (mySerial2.available())
{
  MQTT_connect();
  
  for (i=0;i<5;i++)
  {
    sensor[i] = mySerial2.parseInt();
  }   
  delay(2000);
  vib.publish(sensor[0]);
  delay(2000);
  temp.publish(sensor[1]);
  delay(2000);
  current.publish(sensor[2]);
  delay(2000);
  vol.publish(sensor[3]);
  delay(2000);
  rpm.publish(sensor[4]);
  
  }
}
void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) return;

  //Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) 
  { 
    // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(3000);  // wait 3 seconds
       retries--;
       if (retries == 0) {
         
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


