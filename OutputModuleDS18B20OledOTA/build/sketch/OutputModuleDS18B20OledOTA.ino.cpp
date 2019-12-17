#include <Arduino.h>
#line 1 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
/*
Copyright 2016 German Martin (gmag11@gmail.com). All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met :

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list
of conditions and the following disclaimer in the documentation and / or other materials
provided with the distribution.

THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  The views and conclusions contained in the software and documentation are those of the
  authors and should not be interpreted as representing official policies, either expressed
  or implied, of German Martin
*/

/*
 Name:    NtpClient.ino
 Created: 20/08/2016
 Author:  gmag11@gmail.com
 Editor:  http://www.visualmicro.com
*/

//#define NTP_ON
//#define OLED_ON

/************ Pin definition for use generic module***************/

static const uint8_t D0 = 16;
static const uint8_t D1 = 5;
static const uint8_t D2 = 4;
static const uint8_t D3 = 0;
static const uint8_t D4 = 2;
static const uint8_t D5 = 14;
static const uint8_t D6 = 12;
static const uint8_t D7 = 13;
static const uint8_t D8 = 15;
static const uint8_t RX = 3;
static const uint8_t TX = 1;

// Include the correct display library
// For a connection via I2C using Wire include
#ifdef OLED_ON
#include <Wire.h>   // Only needed for Arduino 1.6.5 and earlier
                    //#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "SH1106.h" // alis for `#include "SH1106Wire.h"`
                    // For a connection via I2C using brzo_i2c (must be installed) include
                    // #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
                    // #include "SSD1306Brzo.h"
                    // #include "SH1106Brzo.h"
                    // For a connection via SPI include
                    // #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
                    // #include "SSD1306Spi.h"
                    // #include "SH1106SPi.h"

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi        display(D0, D2, D8);
// or
// SH1106Spi         display(D0, D2);

// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL
// SSD1306Brzo display(0x3c, D3, D5);
// or
// SH1106Brzo  display(0x3c, D3, D5);

// Initialize the OLED display using Wire library
//SSD1306  display(0x3c, D3, D5);
SH1106 display(0x3c, D1, D2);

#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;
//char aktualnyCzas[100];

/*oled interface */
#endif
#include <TimeLib.h>
//#include "WifiConfig.h"
#include "images.h"
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#ifdef NTP_ON
#include <NtpClientLib.h>
#endif
#ifndef WIFI_CONFIG_H
#define YOUR_WIFI_SSID "NETIASPOT-C61350"
#define YOUR_WIFI_PASSWD "wiktorpatryk"
#endif // !WIFI_CONFIG_H

#define ONE_WIRE_BUS D3 // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float actualTemperature;
String setpointTemperature, roomTemperature;
int noResponseCounter;

#define ONBOARDLED 2 // Built in LED on ESP-12/ESP-07
#define OUTPUT_PIN D0
volatile bool outputState;

WiFiClient client;
bool clientConnected, dataExchangeOk,clientCleared;
const char *host = "192.168.1.27";
int8_t timeZone = 1;

// Start NTP only after IP network is connected
#line 132 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
void onSTAGotIP(WiFiEventStationModeGotIP ipInfo);
#line 143 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
void onSTADisconnected(WiFiEventStationModeDisconnected event_info);
#line 196 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
bool connectToServer(void);
#line 217 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
String findTag(String input, String tag);
#line 238 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
void sendStr(String str);
#line 245 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
void checkClientResponse(void);
#line 297 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
void sendStatus(void);
#line 313 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
void setup();
#line 374 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
void loop();
#line 132 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
void onSTAGotIP(WiFiEventStationModeGotIP ipInfo)
{
  Serial.printf("Got IP: %s\r\n", ipInfo.ip.toString().c_str());
#ifdef NTP_ON
  NTP.begin("pool.ntp.org", timeZone, true);
  NTP.setInterval(63);
#endif
  digitalWrite(ONBOARDLED, LOW); // Turn on LED
}

// Manage network disconnection
void onSTADisconnected(WiFiEventStationModeDisconnected event_info)
{
  Serial.printf("Disconnected from SSID: %s\n", event_info.ssid.c_str());
  Serial.printf("Reason: %d\n", event_info.reason);
  digitalWrite(ONBOARDLED, HIGH); // Turn off LED
  //NTP.stop(); // NTP sync can be disabled to avoid sync errors
}

#ifdef NTP_ON
void processSyncEvent(NTPSyncEvent_t ntpEvent)
{
  if (ntpEvent)
  {
    Serial.print("Time Sync error: ");
    if (ntpEvent == noResponse)
      Serial.println("NTP server not reachable");
    else if (ntpEvent == invalidAddress)
      Serial.println("Invalid NTP server address");
  }
  else
  {
    Serial.print("Got NTP time: ");
    Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
  }
}

boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent;            // Last triggered event
#endif
#ifdef OLED_ON
void drawTime()
{
  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
#ifdef NTP_ON
  display.drawString(86, 0, aktualnyCzas);
#endif
  display.drawString(0, 0, "WODA: " + String(actualTemperature));
  display.drawString(75, 51, "AKT: " + roomTemperature);
  display.drawString(0, 51, "SET: " + setpointTemperature);
  display.setFont(ArialMT_Plain_16);
  outputState ? display.drawString(28, 25, "Piec ON") : display.drawString(28, 25, "Piec OFF");
  if (clientConnected && dataExchangeOk)
    display.drawXbm(62, 1, wifi_width, wifi_height, wifi_bits);
}

Demo demos[] = {drawTime}; //, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;
#endif
//output module functions
bool connectToServer(void)
{

  if (!client.connected())
  {
    if (client.connect(host, 1500))
    {
      Serial.println("client connected"); 
      clientCleared=false;
      return true;
    }
    else
    {
      Serial.println("conneciton faild");
      return false;
    }
  }
  else
    return true;
}

String findTag(String input, String tag)
{

  String startTag = "<" + tag + ">";
  String endTag = "</" + tag + ">";
  String missing = "";
  if (input.indexOf(startTag) >= 0 && input.indexOf(endTag) >= 0)
  {
    Serial.println("znalezniono substring");
    int startIndex = input.indexOf(startTag) + startTag.length();
    int stopIndex = input.indexOf(endTag);
    String value = input.substring(startIndex, stopIndex);
    Serial.println("substring: " + value);
    return value;
  }
  else
  {
    return missing;
  }
}

void sendStr(String str)
{  
  if (client.connected()){
    client.println(str);
  }
}

void checkClientResponse(void)
{
  //<content><setpointTemperature>21.0</setpointTemperature><roomTemperature>22.0</roomTemperature><outputState>ON</outputState></content>
  //<content><setpointTemperature>21.0</setpointTemperature><roomTemperature>22.0</roomTemperature><outputState>OFF</outputState></content>
  
  static int counter = 0;
  String value;
  while (client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
    if (!findTag(line, "setpointTemperature").equals(""))
      setpointTemperature = findTag(line, "setpointTemperature");
    if (!findTag(line, "roomTemperature").equals(""))
      roomTemperature = findTag(line, "roomTemperature");
    value = findTag(line, "outputState");
    String str="Otrzymano: ";
    str+=value;
    if (!value.equals(""))
    {
      if (value.equals("ON"))
      {
        outputState = true;
      }
      if (value.equals("OFF"))
      {
        outputState = false;
      }
    }
    noResponseCounter = 0;
  }
  if (noResponseCounter > 2)
  {
    clientCleared=true;
    outputState = false;
    dataExchangeOk = false;
    client.flush();
    client.stop();
    noResponseCounter = 0;
  }
  else
  {
    dataExchangeOk = true;
  }
  //delay(100);
  counter += 1;
  if (counter > 30)
  {   
    counter = 0;
  }
}

void sendStatus(void)
{
  
  if (client.connected()){
    String dataToSend = "<content><RSSI>" + String(WiFi.RSSI()) + "</RSSI>";
    dataToSend += "<dev_type>outputModule</dev_type>";
    dataToSend += "<id>1002</id>";
    dataToSend += String("<outputState>") + String(outputState ? "ON" : "OFF") + String("</outputState>");
    dataToSend += String("<sensorTemperature>") + String(actualTemperature) + String("</sensorTemperature></content>");
    Serial.println(dataToSend);
    client.println(dataToSend);
    if (noResponseCounter < 20)
        noResponseCounter++;
  }
}

void setup()
{
  pinMode(OUTPUT_PIN, OUTPUT);
  // Initialising the UI will init the display too.
#ifdef OLED_ON
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
#endif
  static WiFiEventHandler e1, e2;

  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.begin(YOUR_WIFI_SSID, YOUR_WIFI_PASSWD);

  pinMode(ONBOARDLED, OUTPUT);    // Onboard LED
  digitalWrite(ONBOARDLED, HIGH); // Switch off LED

#ifdef NTP_ON
  NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
    ntpEvent = event;
    syncEventTriggered = true;
  });
#endif
  // Deprecated
  /*WiFi.onEvent([](WiFiEvent_t e) {
    Serial.printf("Event wifi -----> %d\n", e);
  });*/

  e1 = WiFi.onStationModeGotIP(onSTAGotIP); // As soon WiFi is connected, start NTP Client
  e2 = WiFi.onStationModeDisconnected(onSTADisconnected);

  /************* OTA configuration ***************************/
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.setHostname("outputModule1002");
  ArduinoOTA.begin();
}

void loop()
{
  static int i = 0;
  static int last=0,sendTimer = 0,connectTimer = 0;
  static int lastTime = 0;
  ESP.wdtEnable(10000);
  ArduinoOTA.handle();

#ifdef NTP_ON
  if (syncEventTriggered)
  {
    processSyncEvent(ntpEvent);
    syncEventTriggered = false;
  }
#endif
#ifdef OLED_ON
  display.clear();
  // draw the current demo method
  demos[demoMode]();

  // write the buffer to the display
  display.display();

  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION)
  {
    demoMode = (demoMode + 1) % demoLength;
    timeSinceLastModeSwitch = millis();
  }
#endif
  if ((millis() - last) > 500)
  {
    //Serial.println(millis() - last);
    last = millis();
    /*  Serial.print(i); Serial.print(" ");
    Serial.print(NTP.getTimeDateString()); Serial.print(" ");
    Serial.print(NTP.isSummerTime() ? "Summer Time. " : "Winter Time. ");
    Serial.print("WiFi is ");
    Serial.print(WiFi.isConnected() ? "connected" : "not connected"); Serial.print(". ");
    Serial.print("Uptime: ");
    Serial.print(NTP.getUptimeString()); Serial.print(" since ");
   Serial.println(NTP.getTimeDateString(NTP.getFirstSync()).c_str());*/
#ifdef NTP_ON
    strncpy(aktualnyCzas, NTP.getTimeDateString().c_str(), 9);
#endif
    if (connectToServer())
    {
      clientConnected = true;
      checkClientResponse();
    }
    else
    {
      clientConnected = false;
    }
  }
  if (clientConnected && dataExchangeOk)
    digitalWrite(OUTPUT_PIN, outputState);
  else
  {
    digitalWrite(OUTPUT_PIN, LOW);
    outputState = false;
  }
  /** wylaczony pomiar temperatury*/
  if ((millis() - lastTime) > 5000)
  {
    lastTime = millis();
    do
    {
      DS18B20.requestTemperatures();
      actualTemperature = DS18B20.getTempCByIndex(0);
    } while ((actualTemperature == 85.0 || actualTemperature == (-127.0)) && ((millis() - lastTime) < 3000));
  }

  
  if ((millis() - sendTimer) > 10000)
  {
    sendTimer = millis();
      sendStatus();
     // digitalRead(OUTPUT_PIN) ? digitalWrite(OUTPUT_PIN,LOW) : digitalWrite(OUTPUT_PIN,HIGH);
  }
  delay(0);
  ESP.wdtFeed();
}

