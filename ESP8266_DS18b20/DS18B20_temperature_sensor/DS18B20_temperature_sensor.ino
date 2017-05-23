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

//AP definitions
#define AP_SSID "NETIASPOT-C61350"
#define AP_PASSWORD "wiktorpatryk"

const char* host = "data.sparkfun.com";
const char* streamId   = "ZGWgDRxzN9uwqryqpr3y";
const char* privateKey = "2mzAoN5W2dhM9Ra9NR6a";


#define REPORT_INTERVAL 60 // in sec


#define ONE_WIRE_BUS 2  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


#define USER_PWD_LEN 40
char unameenc[USER_PWD_LEN];
float oldTemp,temp;


void setup() {
  Serial.begin(115200);
  
  wifiConnect();
    
 // char uname[USER_PWD_LEN];
///  String str = String(EIOT_USERNAME)+":"+String(EIOT_PASSWORD);  
//  str.toCharArray(uname, USER_PWD_LEN); 
//  memset(unameenc,0,sizeof(unameenc));
//  base64_encode(unameenc, uname, strlen(uname));
  oldTemp = -1;
}

void loop() {
 /* float temp;
  
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.println(temp);
  } while (temp == 85.0 || temp == (-127.0));
  
  if (temp != oldTemp)
  {
    sendTeperature(temp);
    oldTemp = temp;
  }*/
  temp+=1;
  sendTemperature(temp);
  int cnt = REPORT_INTERVAL;
  
  while(cnt--)
    delay(1000);
}

void wifiConnect()
{
    Serial.print("Connecting to AP");
    WiFi.begin(AP_SSID, AP_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
}

void sendTemperature(float temperature)
{  
   WiFiClient client;
   
   while(!client.connect(host, 80)) {
    Serial.println("connection failed");
    wifiConnect(); 
  }
 

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

  delay(100);
    while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("Connection closed");
 // Serial.println("ESP8266 in sleep mode");
 // ESP.deepSleep(60 * 1000000);
}

