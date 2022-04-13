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

#define   STATION_SSID     ""
#define   STATION_PASSWORD ""

#define HOSTNAME "HTTP_BRIDGE"

#include "SmartObjectMain.hpp"

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

auto value = SO.makeSmartValue("light1", 
[&](const String& event, String& value){
  if(event == ""){
    if(value == "on"){
      value = "off";
    }else{
      value = "on";
    }
  }

},
[&](const String& value){
  if (value == "on"){
    digitalWrite(D0, LOW);
  }else{
    digitalWrite(D0, HIGH);
  }
}
);

void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT);

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
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 6 );
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

  server.on("/api", HTTP_POST, [](AsyncWebServerRequest *request){

    
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
