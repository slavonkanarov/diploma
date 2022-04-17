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

#define   STATION_SSID     "grrrnet"
#define   STATION_PASSWORD "pt10mzx6"

#define HOSTNAME "HTTP_BRIDGE"

#include "SmartObjectMain.hpp"

// Prototype
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
String getContentType(String);
bool handleFileRead(AsyncWebServerRequest*);
IPAddress getlocalIP();

painlessMesh  mesh;
AsyncWebServer server(80);
IPAddress myIP(0,0,0,0);
IPAddress myAPIP(0,0,0,0);

SmartObjectMain SO(&mesh);

bool ignoreMode = true;
uint32_t nodeToIgnore1 = 3662020501;
uint32_t nodeToIgnore2 = 3662027117;

void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT);

  // matrix.setAll(mode, brightness, red, green, blue);
  // matrix.show();

    // Initialize SPIFFS
  if (!SPIFFS.begin()){ Serial.println("Failed to mount SPIFFS");}
  else{SO.loadSettings();}

  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION | DEBUG);
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 6);
  mesh.stationManual(STATION_SSID, STATION_PASSWORD);
  mesh.setHostname(HOSTNAME);
  mesh.setRoot(true);
  mesh.setContainsRoot(true);

  SO.initMesh();

  myAPIP = IPAddress(mesh.getAPIP());
  Serial.println("My AP IP is " + myAPIP.toString());



//---------------------------------------------------------------------------
  //Async webserver
  server.on("/buf", HTTP_GET, [](AsyncWebServerRequest *request){
    String text = mesh.asNodeTree().toString();
    request->send(200, "text/html", text.c_str());
  });

  server.on("/api/set/systemMode", HTTP_GET, [](AsyncWebServerRequest *request){
    SO.systemMode(request->arg("systemMode"));;
    request->send(200, "text/plain", "OK");
  });

  server.on("/api/set/ignoreMode", HTTP_GET, [](AsyncWebServerRequest *request){
    ignoreMode =  request->arg("ignoreMode").toInt();
    SO.ignoreConnectionState(nodeToIgnore1, nodeToIgnore2, ignoreMode);
    request->send(200, "text/plain", "OK");
  });

  server.on("/api/get/systemMode", HTTP_GET, [](AsyncWebServerRequest *request){
    DynamicJsonDocument data(64);
    data["ignoreMode"] = SO.getSystemMode();
    String out;
    serializeJson(data, out);
    request->send(200, "text/plain", out);
  });

  server.on("/api/get/ignoreMode", HTTP_GET, [](AsyncWebServerRequest *request){
    DynamicJsonDocument data(64);
    data["ignoreMode"] = ignoreMode;
    String out;
    serializeJson(data, out);
    request->send(200, "text/plain", out);
  });


  server.onNotFound([](AsyncWebServerRequest *request){
    if (!handleFileRead(request))
          request->send(404, "text/plain", "FileNotFound");
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

//Web
String getContentType(String filename) {
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(AsyncWebServerRequest *request) {
  String path = request->url();
  if (path.endsWith("/")) path += "index.html";
  if(path[0] != '/')path = String("/") + path;
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    request->send(SPIFFS, path, contentType);
    return true;
  }
  return false;
}