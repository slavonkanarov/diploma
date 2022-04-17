//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
#include "painlessMesh.h"
#include <Arduino.h>
#include "SmartObjectBasic.hpp"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

SmartObjectBasic SO(&mesh);

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

  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION | DEBUG ); 
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
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
  
  mesh.update();
  if (millis() - t > 3000){
    Serial.print(WiFi.RSSI());
    Serial.printf(WiFi.SSID().c_str());
    Serial.printf("\n");
    t = millis();
    Serial.print("\n");
  }
  send();
}
