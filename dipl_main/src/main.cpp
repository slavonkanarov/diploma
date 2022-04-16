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

#ifdef ESP8266
#include "Hash.h"
#include "FS.h"
#include <ESPAsyncTCP.h>
#else
#include "SPIFFS.h"
#include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#define   STATION_SSID     "ssid_main"
#define   STATION_PASSWORD "1234567890"

#define HOSTNAME "HTTP_BRIDGE"

#include "SmartObjectMain.hpp"

//lamp settings
  // uint8_t green = GREEN;
  // uint8_t red   = RED;
  // uint8_t blue  = BLUE;
  // uint8_t brightness = BRIGHTNESS;
  // uint8_t mode = MODE;
  // Settings matrix;

// Prototype
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
IPAddress getlocalIP();

painlessMesh  mesh;
AsyncWebServer server(80);
IPAddress myIP(0,0,0,0);
IPAddress myAPIP(0,0,0,0);

SmartObjectMain SO(&mesh);


// auto value = SO.makeSmartValue("light1", //имя переменной
// [](const String& event, String& value){
// /*
// функция обрабатывает приходящий ивент сформированный сценарием и должна обновить состояние value
// */
//   if(event == ""){
//     if(value == "on"){
//       value = "off";
//     }else{
//       value = "on";
//     }
//   }
// },
// [](const String& value){
// /*
// функция обрабатывает состояние value и принимает действие на основе этого
// */
//   if (value == "on"){
//     digitalWrite(D0, LOW);
//     brightness = 255;
//   }else{
//     digitalWrite(D0, HIGH);
//     brightness = 0;
//   }
// });


  

void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT);

  // matrix.setAll(mode, brightness, red, green, blue);
  // matrix.show();

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
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 1 );
  mesh.stationManual(STATION_SSID, STATION_PASSWORD);
  mesh.setHostname(HOSTNAME);
  mesh.setRoot(true);
  mesh.setContainsRoot(true);

  SO.initMesh();

  myAPIP = IPAddress(mesh.getAPIP());
  Serial.println("My AP IP is " + myAPIP.toString());



//---------------------------------------------------------------------------
  //Async webserver
  server.on("/admin", HTTP_GET, [](AsyncWebServerRequest *request){
    String text = String("");

    text += mesh.asNodeTree().toString();
      
    request->send(200, "text/html", text.c_str());
  });

  server.on("/buf", HTTP_GET, [](AsyncWebServerRequest *request){
    String text = String("");

    text += mesh.asNodeTree().toString();
    
    request->send(200, "text/html", text.c_str());
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String text = String(" "\
                         "");

    text += mesh.asNodeTree().toString();

    if (request->hasArg("BROADCAST")){
      String msg = request->arg("BROADCAST");
      mesh.sendBroadcast(msg, true);
      
    };

    request->send(SPIFFS, "/index.html", String());
  });

  server.on("/api", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasArg("systemMode")){
      SO.systemMode(request->arg("systemMode"));
       Serial.println("systemMod " + request->arg("systemMode"));
    };
    
    request->send(SPIFFS, "/index.html", String());
  });

  server.on("/api", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasArg("systemMode")){
      SO.systemMode(request->arg("systemMode"));
       Serial.println("systemMod " + request->arg("systemMode"));
    };
    
    request->send(200, "text/html", "");
  });

  server.begin();

}

void loop() {
  mesh.update();
  if(myIP != getlocalIP()){
    myIP = getlocalIP();
    Serial.println("My IP is " + myIP.toString());
  }
  // matrix.setAll(mode, brightness, red, green, blue); 
  // matrix.show();
  // delay(20);
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

IPAddress getlocalIP() {
  return IPAddress(mesh.getStationIP());
}
