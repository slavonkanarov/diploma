
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPUpdateServer.h>


#define   STATION_SSID     "grrrnet"
#define   STATION_PASSWORD "pt10mzx6"

#define HOSTNAME "HTTP_BRIDGE"

#include "SmartObjectMain.hpp"

String getContentType(String);
bool handleFileRead();
IPAddress getlocalIP();

ESP8266HTTPUpdateServer httpUpdater;
ESP8266WebServer HTTP;
SmartObjectMain SO(&HTTP);

bool ignoreMode = true;
uint32_t nodeToIgnore1 = 3662020501;
uint32_t nodeToIgnore2 = 3662027117;

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

void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT);

  if (!SPIFFS.begin()){ Serial.println("Failed to mount SPIFFS");}
  else{SO.loadSettings();}

  httpUpdater.setup(&HTTP);
  WIFIinit();

  SO.initMesh();

//---------------------------------------------------------------------------

  HTTP.on("/api/set/systemMode", HTTP_GET, [](){
    SO.systemMode(HTTP.arg("systemMode"));;
    HTTP.send(200, "text/plain", "OK");
  });

  HTTP.on("/api/set/ignoreMode", HTTP_GET, [](){
    ignoreMode =  HTTP.arg("ignoreMode").toInt();
    SO.ignoreConnectionState(nodeToIgnore1, nodeToIgnore2, ignoreMode);
    HTTP.send(200, "text/plain", "OK");
  });

  HTTP.on("/api/get/systemMode", HTTP_GET, [](){
    DynamicJsonDocument data(64);
    data["ignoreMode"] = SO.getSystemMode();
    String out;
    serializeJson(data, out);
    HTTP.send(200, "text/plain", out);
  });

  HTTP.on("/api/get/ignoreMode", HTTP_GET, [](){
    DynamicJsonDocument data(64);
    data["ignoreMode"] = ignoreMode;
    String out;
    serializeJson(data, out);
    HTTP.send(200, "text/plain", out);
  });


  HTTP.onNotFound([](){
    if (!handleFileRead())
          HTTP.send(404, "text/plain", "FileNotFound");
  });

  HTTP.begin();

}

void loop() {
  if(WiFi.status() != WL_CONNECTED && WiFi.getMode()==WIFI_STA){
    WIFIinit();
  }
  HTTP.handleClient();
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

bool handleFileRead() {
  String path = HTTP.uri();
  if (path.endsWith("/")) path += "index.html";
  if(path[0] != '/')path = String("/") + path;
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = HTTP.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}