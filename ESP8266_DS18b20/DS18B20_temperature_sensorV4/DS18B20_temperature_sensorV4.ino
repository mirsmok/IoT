 /*
  Created by Igor Jarc
 See http://iot-playground.com for details
 Please use community fourum on website do not contact author directly
 
 Code based on https://github.com/DennisSc/easyIoT-ESPduino/blob/master/sketches/ds18b20.ino
 
 External libraries:
 - https://github.com/adamvr/arduino-base64
 - https://github.com/milesburton/Arduino-Temperature-Control-Library
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

 
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

////AP definitions
//#define AP_SSID "FunBox2-5751"
//#define AP_PASSWORD "E57D71222F4916E965F4D57C3E"


//#define AP_SSID "Xperia Z2_2993"
//#define AP_PASSWORD "mirsmok82"

#define AP_SSID "NETIASPOT-C61350"
#define AP_PASSWORD "wiktorpatryk"

//sparkfun.com settings
//const char* host = "data.sparkfun.com";
//const char* streamId   = "ZGWgDRxzN9uwqryqpr3y";
//const char* privateKey = "2mzAoN5W2dhM9Ra9NR6a";

//thinkspeak.com settings
const char* host = "192.168.1.201";
//const int channelID = XXX;
String writeAPIKey = "HPGNR50XGF9V9KAD"; // write API key for your ThingSpeak Channel


#define ONE_WIRE_BUS 14  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


#define USER_PWD_LEN 40
char unameenc[USER_PWD_LEN];
float oldTemp;
// Time to sleep (in seconds):
int sleepTimeS;

void setup() {
  sleepTimeS=600;
  // zasilanie przetwornika temperatury ds18b20
  pinMode(16, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(16, HIGH);
  digitalWrite(2, LOW);
  //wylaczenie ledow na plytce
  pinMode(4,INPUT);
  digitalWrite(4, LOW);
  pinMode(5,OUTPUT);
  digitalWrite(5, LOW);
  pinMode(12,INPUT);
//  digitalWrite(12, LOW);
  pinMode(13,INPUT);
//  digitalWrite(13, LOW);
  pinMode(15,INPUT);
//  digitalWrite(15, LOW);

  
  
  Serial.begin(115200);
  wifiConnect();
    
  float temp,analogV;
  int analog;
  long rssi;
  
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
  } while (temp == 85.0 || temp == (-127.0));
    Serial.println("");
    Serial.print("Temperature: ");
    Serial.println(temp);
    analog=analogRead(A0);
    Serial.print("Analog: ");
    Serial.println(analog);
    analogV=0.01075*((float)analog);
    Serial.print("Analog [V]: ");
    Serial.println(analogV);
    rssi = WiFi.RSSI();
    sendResults(temp,analogV,rssi);
    
      // Sleep
      Serial.println("a");
  Serial.println("ESP8266 in sleep mode");
  
    pinMode(2,INPUT);
    pinMode(4,INPUT);
    pinMode(5,INPUT);
    pinMode(12,INPUT);
    pinMode(13,INPUT);
    pinMode(14,INPUT);
    pinMode(15,INPUT);
    pinMode(16,INPUT);
  ESP.deepSleep(sleepTimeS * 1000000);
}

void loop() {
 
}

void wifiConnect()
{
    Serial.print("Connecting to AP");
    WiFi.begin(AP_SSID,AP_PASSWORD);
    int connectAttemps=0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
      digitalWrite(5,!digitalRead(5));
      connectAttemps++;
      if(connectAttemps > 10){
        Serial.println("Can't connect to Wifi go to Sleep for 20 minuts...");
        sleepTimeS=1200;
        ESP.deepSleep(sleepTimeS * 1000000);
        }
    }  
  Serial.println("");
  Serial.println("WiFi connected");  
}

void sendResults(float temperature,float supplayVoltage,long rssi)
{  
   WiFiClient client;
   
   while(!client.connect(host, 1500)) {
    Serial.println("connection failed");
    wifiConnect(); 
  }
 
/* sparkfun send data
   // We now create a URI for the request
  String url = "/input/";
  url += streamId;
  url += "?private_key=";
  url += privateKey;
  url += "&temp=";
  url += String(temperature);
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
*/
/* thinkSpea
 String body = "field1=";
           body += String(temperature);
           body += String("&field2=");
           body += String(supplayVoltage);
           body += String("&field3=");
           body += String(rssi);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(body.length());
    client.print("\n\n");
    client.print(body);
    client.print("\n\n");
    
    Serial.println(body);
    */
    String dataToSend = "<content><RSSI>"+String(WiFi.RSSI())+"</RSSI>";
    dataToSend+="<dev_type>temperatureSensor</dev_type>";
    dataToSend+="<supplayVoltage>"+String(supplayVoltage)+"</supplayVoltage>";
    dataToSend+="<sensorTemperature>"+String(temperature)+"</sensorTemperature>";
    dataToSend+="<id>2001</id></content>";
    client.println(dataToSend);
  delay(100);
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  client.flush();
  client.stop();
  Serial.println();
  Serial.println("Connection closed");
}

