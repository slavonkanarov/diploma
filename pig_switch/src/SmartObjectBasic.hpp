#ifndef _SmartObjectBasic_
#define _SmartObjectBasic_
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPUpdateServer.h>
#include "painlessMesh.h"
#include <vector>
#include <functional>
#include <string>
#include <ESP8266HTTPClient.h>

#include "SmartValue.hpp"
#include "SmartActivator.hpp"
#include "Scenario.hpp"

using namespace std;

class SmartObjectBasic{
protected:
    ESP8266WebServer*  mesh;
    StaticJsonDocument<512> settings;
    vector<Scene> scenes;
    vector<pair<uint32_t, bool>> ignoreList;//default state false
    vector<SmartValue> values;
    vector<SmartActivator> pub;

    String systemModeState = "local";

    function<void(const String &)> receivedCallback = [this]( const String &msg ){
        Serial.printf("bridge: Received msg=%s\n", msg.c_str());
        
        DynamicJsonDocument data(256);
        deserializeJson(data, msg);

        //System msg
        if(data["command"].as<String>() == "nodeWorking"){

        }else

        if(data["command"].as<String>() == "systemMode"){
            this->systemModeState = data["mode"].as<String>();
        }else
        
        if(data["command"].as<String>() == "ignoreNodeState"){
            this->setIgnoreState(data["ignoredNode"].as<uint32_t>(), data["state"].as<uint8_t>());
        }else

        //Delete msg zone

        if(this->getIgnoreState(data["from"].as<uint32_t>())){
            return;
        }else

        //Node msg
        if(data["command"].as<String>() == "sendEventToExecutor"){
            if(data["target"].as<uint32_t>() != uint32_t(WiFi.localIP())){
                this->sendEventToExecutor(data["target"].as<uint32_t>(), data["executor"].as<String>(), data["event"].as<String>());
            }else{
                Serial.printf("trying to find executor");
                String executor = data["executor"].as<String>();
                for(uint32_t i = 0; i < this->values.size(); ++i){
                    if(this->values[i].getName() == executor){
                        Serial.printf("executor is finded");
                        this->values[i].processEvent(data["event"].as<String>());
                    }
                }
            }
        }else
        
        if(data["command"].as<String>() == "sendEventToRoot"){
            String activator = data["activator"].as<String>();
            if(settings["mesh"]["root"].as<uint32_t>() == 1){
                for(uint32_t i = 0; i < this->scenes.size(); ++i){
                    if(this->scenes[i].activator == activator){
                        Serial.printf("find scene: activator=%s; event=%s; executor=%s; target=%u;\n",
                                     this->scenes[i].activator.c_str(),
                                     this->scenes[i].event.c_str(),
                                     this->scenes[i].executor.c_str(),
                                     this->scenes[i].target);
                        this->sendEventToExecutor(this->scenes[i].target, this->scenes[i].executor, this->scenes[i].event);
                    }
                }
            }else{
                this->sendEventToRoot(activator);
            }
        }
    };

    function<void(const SmartActivator*)> analizer = [this](const SmartActivator* sp){
        if (this->systemModeState == "local"){
            for(uint32_t i = 0; i < this->scenes.size(); ++i){
                if(this->scenes[i].activator == sp->getName()){
                    Serial.printf("find scene: activator=%s; event=%s; executor=%s; target=%u;\n",
                        this->scenes[i].activator.c_str(),
                        this->scenes[i].event.c_str(),
                        this->scenes[i].executor.c_str(),
                        this->scenes[i].target);
                    this->sendEventToExecutor(this->scenes[i].target, this->scenes[i].executor, this->scenes[i].event);
                }
            }
        }else{
             this->sendEventToRoot(sp->getName());
        }
    };

public:

    void sendSingle(const uint32_t& to, const String& msg){
        WiFiClient client;
        HTTPClient http;

        String serverPath = IPAddress(to).toString() + "/server";
        
        // Your Domain name with URL path or IP address with path
        http.begin(client, serverPath.c_str());
        
        // Send HTTP GET request
        int httpResponseCode = http.POST(msg);
    }

    SmartObjectBasic(ESP8266WebServer*  mesh_object){
        mesh = mesh_object;
    };

    void initMesh(){
          mesh->on("/server", HTTP_POST, [this](){this->receivedCallback(mesh->arg("plain"));});
    };

    void loadSettings(){
        Serial.println("---OPEN SETTINGS---");
        File file = SPIFFS.open("/settings.json", "r");

        if (!file)
        {
            Serial.println("There was an error opening the file");
            return;
        }else{
            Serial.println("File opened!");
            DeserializationError error = deserializeJson(settings, file);
            if (error)
            {
                Serial.println("error...");
            }else{
                Serial.println("deserialize settings");
            }
        }

        file.close();
        Serial.println("---SETTINGS LOADED---\n");

        loadScenes();
    }

    void loadScenes(){
        for(int i = 0; i < settings["scenes"].size(); ++i){
            Scene sc = Scene();
            sc.activator = settings["scenes"][i]["activator"].as<String>();
            sc.event = settings["scenes"][i]["event"].as<String>();
            sc.executor = settings["scenes"][i]["executor"].as<String>();
            sc.target = settings["scenes"][i]["target"].as<uint32_t>();

            scenes.push_back(sc);
            Serial.printf("added scene: activator=%s; event=%s; executor=%s; target=%u;\n", sc.activator.c_str(), sc.event.c_str(), sc.executor.c_str(), sc.target);
        }
    }

    bool getIgnoreState(const uint32_t& node){
        for(uint32_t i = 0; i < ignoreList.size(); ++i){
            if(node == ignoreList[i].first){
                return ignoreList[i].second;
            }
        }
        return false;
    }

    void setIgnoreState(const uint32_t& node, bool state){
        for(uint32_t i = 0; i < ignoreList.size(); ++i){
            if(node == ignoreList[i].first){
                ignoreList[i].second = state;
                return;
            }
        }
        ignoreList.emplace_back(node, state);
    }

    void sendEventToExecutor(const uint32_t& target, const String& executor, const String& event){
        DynamicJsonDocument data(128);

        data["from"] = uint32_t(WiFi.localIP());
        data["command"] = "sendEventToExecutor";
        data["target"] = target;
        data["executor"] = executor;
        data["event"] = event;

        String out;
        serializeJson(data, out);
        if(target == uint32_t(WiFi.localIP())){
            receivedCallback(out);
            return;
        }

        for(uint32_t i = 0; i < settings["mesh"]["childs"].size(); ++i){
            uint32_t node = settings["mesh"]["childs"][i]["nodeId"].as<uint32_t>();
            if(node == target){
                sendSingle(node, out);
                return;
            }
            for(uint32_t j = 0; j < settings["mesh"]["childs"][i]["subs"].size(); ++j){
                node = settings["mesh"]["childs"][i]["subs"].as<uint32_t>();
                if(node == target){
                    sendSingle(node, out);
                    Serial.printf("Resend to: %u", target);
                    return;
                }
            }
        }
        sendSingle(settings["mesh"]["parent"].as<uint32_t>(), out);
    }

    void sendEventToRoot(const String& activator){
        DynamicJsonDocument data(128);

        data["from"] = uint32_t(WiFi.localIP());
        data["command"] = "sendEventToRoot";
        data["activator"] = activator;

        String out;
        serializeJson(data, out);

        sendSingle(settings["mesh"]["parent"].as<uint32_t>(), out);
    }

    SmartValue* makeSmartValue(const String& name_of_value, 
                const function<void(const String&, String&)>& parseEvent_setNewValue,//func(event, &value)
                const function<void(const String&)>& processValue_doSometing//func(value)
                ){
        values.emplace_back(name_of_value, parseEvent_setNewValue, processValue_doSometing);
        return &values.back();
    }

    SmartActivator* makeSmartActivator(const String& name_of_activator){
        pub.emplace_back(name_of_activator, this->analizer);
        return &pub.back();
    }
};

#endif