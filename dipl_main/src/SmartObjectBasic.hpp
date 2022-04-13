#ifndef _SmartObjectBasic_
#define _SmartObjectBasic_

#include "painlessMesh.h"
#include<vector>
#include <functional>
#include <string>

#include "SmartValue.hpp"
#include "SmartPublisher.hpp"
#include "Scenario.hpp"

using namespace std;

class SmartObjectBasic{
protected:
    painlessMesh*  mesh;
    StaticJsonDocument<512> settings;
    vector<Scenario> scenes;
    vector<SmartValue> values;
    vector<SmartPublisher> pub;

    String systemMode = "local";

    function<void(const uint32_t &, const String &)> receivedCallback = [this](const uint32_t &from, const String &msg ){
        Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
        
        DynamicJsonDocument data(512);
        deserializeJson(data, msg);
        if(data["command"].as<String>() == "nodeWorking"){

        }else

        if(data["command"].as<String>() == "systemMode"){
            this->systemMode = data["state"].as<String>();
        }else

        if(data["command"].as<String>() == "event"){
            if(data["target"].as<uint32_t>() != this->mesh->getNodeId()){
                this->sendEvent(data["target"].as<uint32_t>(), data["executor"].as<String>(), data["event"].as<String>());
            }else{
                for(uint32_t i = 0; i < this->scenes.size(); ++i){
                    if(this->values[i].getName() == data["executor"].as<String>()){
                        this->values[i].processEvent(data["event"].as<String>());
                    }
                }
            }
        }

    };

    function<void(const SmartPublisher*, const String&)> analizer = [this](const SmartPublisher* sp, const String& event){

            for(uint32_t i = 0; i < this->scenes.size(); ++i){
                if(this->scenes[i].activatedPublisherName == sp->getName()){
                    if(this->scenes[i].activatedEvent == "" || this->scenes[i].activatedEvent == event){
                        this->sendEvent(this->scenes[i].target, this->scenes[i].executorValueName, event);
                    }
                }
            }


    };

public:

    SmartObjectBasic(painlessMesh*  mesh_object){
        mesh = mesh_object;
    };

    void initMesh(){
        mesh->onReceive(receivedCallback);
    };

    void loadSettings(){
        Serial.println("---OPEN SETTINGS---");
        File file = SPIFFS.open("/settings.json", "r");

        if (!file)
        {
            Serial.println("There was an error opening the file");
            return;
        }

        else
        {
            Serial.println("File opened!");
            DeserializationError error = deserializeJson(settings, file);
            if (error)
            {
                Serial.println("error...");
            }
            else
            {
                Serial.println("deserialize settings");
            }
            Serial.println("");
        }

        file.close();
        Serial.println("");


    }

    void sendEvent(const uint32_t& target, const String& executor, const String& event){
        if (this->systemMode == "local"){

        }else{
            
        }
    }

};

#endif