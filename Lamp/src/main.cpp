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

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#include "SmartObjectBasic.hpp"

//lamp settings
uint8_t green = GREEN;
uint8_t red   = RED;
uint8_t blue  = BLUE;
uint8_t brightness = BRIGHTNESS;
uint8_t mode = MODE;
Settings matrix;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

SmartObjectBasic SO(&mesh);


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
    brightness = 255;
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

  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION | DEBUG);
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.setContainsRoot(true);

  SO.initMesh();

}

void loop() {
  mesh.update();
  matrix.setAll(mode, brightness, red, green, blue); 
  matrix.show();
  delay(20);
}