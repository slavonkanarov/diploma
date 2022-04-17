//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************

#include <Arduino.h>
#include "SmartObjectBasic.hpp"

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

auto button1 = SO.makeSmartActivator("buttonPig.click");
int button_pin = D1;
int leed_pin = D2;

//при вызове этой переменной будут обработаны сценарии с активатором button1.click

void setup() {
  Serial.begin(115200);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(leed_pin, OUTPUT);
  digitalWrite(leed_pin, HIGH);
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

bool state = digitalRead(button_pin);
void send() {
  if(state != digitalRead(button_pin)){
    state = !state;
    button1->publish();//активируем сценарий
    digitalWrite(leed_pin, state);
  }
}

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
  send();
}
