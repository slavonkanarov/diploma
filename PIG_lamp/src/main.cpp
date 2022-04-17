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
#include "Consts.h"
#include "Settings.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#include "SmartObjectBasic.hpp"

// Pig lamp
int r__ = 0xff;
int g__ = 0x4d;
int b__ = 0x00;
int fade = 3;
Settings leds;


Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

SmartObjectBasic SO(&mesh);


auto value = SO.makeSmartValue("lightPig", //имя переменной
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
    // digitalWrite(D0, LOW);
    leds.setAll(0, 0, 0, fade);
  }else{
    // digitalWrite(D0, HIGH);
    leds.setAll(r__, g__, b__, fade);
  }
});


void setup() {
  Serial.begin(115200);
  // pinMode(D0, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  analogWrite(redLED, r__);
  analogWrite(greenLED, g__);
  analogWrite(blueLED, b__);
  delay(100);
  leds.setAll(r__, g__, b__, fade);

    // Initialize SPIFFS
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount SPIFFS");
  }
  else
  {
    SO.loadSettings();
  }

  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION | DEBUG);
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);

  SO.initMesh();

}
auto t = millis();
void loop() {
  mesh.update();
  if (millis() - t > 3000){
    Serial.print(WiFi.RSSI());
    Serial.printf(WiFi.SSID().c_str());
    Serial.printf("\n");
    t = millis();
    Serial.print("\n");
  }
  leds.show();
}