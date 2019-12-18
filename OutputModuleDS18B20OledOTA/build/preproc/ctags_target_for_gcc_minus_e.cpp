# 1 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
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
# 29 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
/*

 Name:    NtpClient.ino

 Created: 20/08/2016

 Author:  gmag11@gmail.com

 Editor:  http://www.visualmicro.com

*/
# 36 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
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
# 102 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
# 103 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino" 2
//#include "WifiConfig.h"
# 105 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino" 2
# 106 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino" 2
# 107 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino" 2
# 108 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino" 2
# 109 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino" 2
# 110 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino" 2
# 111 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino" 2
# 120 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
OneWire oneWire(D3 /* DS18B20 pin*/);
DallasTemperature DS18B20(&oneWire);
float actualTemperature;
String setpointTemperature, roomTemperature;
int noResponseCounter;



volatile bool outputState;

WiFiClient client;
bool clientConnected, dataExchangeOk, clientCleared;
const char *host = "192.168.1.27";
int8_t timeZone = 1;

// Start NTP only after IP network is connected
void onSTAGotIP(WiFiEventStationModeGotIP ipInfo)
{
  Serial.printf("Got IP: %s\r\n", ipInfo.ip.toString().c_str());




  digitalWrite(2 /* Built in LED on ESP-12/ESP-07*/, 0x0); // Turn on LED
}

// Manage network disconnection
void onSTADisconnected(WiFiEventStationModeDisconnected event_info)
{
  Serial.printf("Disconnected from SSID: %s\n", event_info.ssid.c_str());
  Serial.printf("Reason: %d\n", event_info.reason);
  digitalWrite(2 /* Built in LED on ESP-12/ESP-07*/, 0x1); // Turn off LED
  //NTP.stop(); // NTP sync can be disabled to avoid sync errors
}
# 199 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
//output module functions
bool connectToServer(void)
{

  if (!client.connected())
  {
    if (client.connect(host, 1500))
    {
      Serial.println("client connected");
      clientCleared = false;
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
  if (client.connected())
  {
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
    String str = "Otrzymano: ";
    str += value;
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
    clientCleared = true;
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

  if (client.connected())
  {
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

void ledStatusUpdate(void)
{
  static int counter = 0;
  if (counter++ % 2)
  {
    if (outputState)
    {
      digitalWrite(15, 0x0);
      digitalWrite(12, 0x0);
      digitalWrite(13, 0x1);
    }
    else
    {
      if (dataExchangeOk && clientConnected)
      {
        digitalWrite(15, 0x0);
        digitalWrite(12, 0x1);
        digitalWrite(13, 0x0);
      }
      else
      {
        digitalWrite(15, 0x1);
        digitalWrite(12, 0x0);
        digitalWrite(13, 0x0);
      }
    }
  }
  else
  {
    digitalWrite(15, 0x0);
    digitalWrite(12, 0x0);
    digitalWrite(13, 0x0);
  }
}

void setup()
{
  pinMode(5, 0x01);
  pinMode(15, 0x01);
  pinMode(12, 0x01);
  pinMode(13, 0x01);
  digitalWrite(15, 0x0);
  digitalWrite(12, 0x0);
  digitalWrite(13, 0x0);
  // Initialising the UI will init the display too.






  static WiFiEventHandler e1, e2;

  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.begin("NETIASPOT-C61350", "wiktorpatryk");

  pinMode(2 /* Built in LED on ESP-12/ESP-07*/, 0x01); // Onboard LED
  digitalWrite(2 /* Built in LED on ESP-12/ESP-07*/, 0x1); // Switch off LED







  // Deprecated
  /*WiFi.onEvent([](WiFiEvent_t e) {

    Serial.printf("Event wifi -----> %d\n", e);

  });*/
# 391 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
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
  static unsigned long last = 0, sendTimer = 0, connectTimer = 0;
  static unsigned long lastTime = 0;
  ESP.wdtEnable(10000);
  ArduinoOTA.handle();
# 450 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
  if ((millis() - last) > 1000)
  {
    ledStatusUpdate();
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
# 466 "c:\\Users\\mirsmok\\work\\IoT\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA\\OutputModuleDS18B20OledOTA.ino"
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
    digitalWrite(5, outputState);
  else
  {
    digitalWrite(5, 0x0);
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
    } while ((actualTemperature == 85.0 || actualTemperature == (-127.0)) && ((millis() - lastTime) < 1000));
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
