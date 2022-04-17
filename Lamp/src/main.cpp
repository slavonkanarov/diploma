//************************************************************
// this is a simple example that uses the painlessMesh library to
// connect to a another network and broadcast message from a webpage to the edges of the mesh network.
// This sketch can be extended further using all the abilities of the AsyncWebserver library (WS, events, ...)
// for more details
// https://gitlab.com/painlessMesh/painlessMesh/wikis/bridge-between-mesh-and-another-network
// for more details about my version
// https://gitlab.com/Assassynv__V/painlessMesh
// and for more details about the AsyncWebserver library
// https://github.com/me-no-dev/ESPAsyncWebServer
//************************************************************

#include "IPAddress.h"
#include "painlessMesh.h"
#include "Timer.h"
#include "Settings.h"
#include "LampConsts.h"
#include "WiFiConsts.h"
#include "hexConverter.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPUpdateServer.h>

#include "SmartObjectBasic.hpp"

#define   STATION_SSID     "grrrnet"
#define   STATION_PASSWORD "pt10mzx6"

String getContentType(String);
bool handleFileRead();
IPAddress getlocalIP();

ESP8266HTTPUpdateServer httpUpdater;
ESP8266WebServer HTTP;
SmartObjectBasic SO(&HTTP);

void WIFIinit() {
  // Попытка подключения к точке доступа
  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STATION_SSID, STATION_PASSWORD);
  // Делаем проверку подключения до тех пор пока счетчик tries
  // не станет равен нулю или не получим подключение
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("ID Node: ");
  Serial.println(uint32_t(WiFi.localIP()));
}


//lamp settings
uint8_t green = GREEN;
uint8_t red   = RED;
uint8_t blue  = BLUE;
uint8_t brightness = BRIGHTNESS;
uint8_t mode = MODE;
Settings matrix;

auto value = SO.makeSmartValue("light1", //имя переменной
[](const String& event, String& value){
/*
функция обрабатывает приходящий ивент сформированный сценарием и должна обновить состояние value
*/
  if(event == ""){
    if(value == "on"){
      value = "off";
    }else{
      value = "on";
    }
  }
},
[](const String& value){
/*
функция обрабатывает состояние value и принимает действие на основе этого
*/
  if (value == "on"){
    digitalWrite(D0, LOW);
    brightness = BRIGHTNESS;
  }else{
    digitalWrite(D0, HIGH);
    brightness = 0;
  }
});


void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT);

  matrix.setAll(mode, brightness, red, green, blue);
  matrix.show();

    // Initialize SPIFFS
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount SPIFFS");
  }
  else
  {
    SO.loadSettings();
  }


  httpUpdater.setup(&HTTP);
  WIFIinit();
  SO.initMesh();

}
auto t = millis();
void loop() {
  if(WiFi.status() != WL_CONNECTED && WiFi.getMode()==WIFI_STA){
    WIFIinit();
  }
  HTTP.handleClient();
  if (millis() - t > 3000){
    Serial.print(WiFi.RSSI());
    Serial.printf(WiFi.SSID().c_str());
    Serial.printf("\n");
    t = millis();
    Serial.print("\n");
  }
  matrix.setAll(mode, brightness, red, green, blue); 
  matrix.show();
  // delay(20);
}