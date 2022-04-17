#ifndef _SmartObjectMain_
#define _SmartObjectMain_

#include "SmartObjectBasic.hpp"

class SmartObjectMain: public SmartObjectBasic{
protected:

public:

    SmartObjectMain(ESP8266WebServer*  mesh_object):SmartObjectBasic(mesh_object){};

    void systemMode(const String& mode){
        DynamicJsonDocument data(64);

        data["command"] = "systemMode";
        data["mode"] = mode;

        String out;
        serializeJson(data, out);

        for(uint32_t i = 0; i < settings["mesh"]["childs"].size(); ++i){
            uint32_t node = settings["mesh"]["childs"][i]["nodeId"].as<uint32_t>();
            sendSingle(node, out);
            Serial.printf("Resend to: %u", node);
            for(uint32_t j = 0; j < settings["mesh"]["childs"][i]["subs"].size(); ++j){
                node = settings["mesh"]["childs"][i]["subs"].as<uint32_t>();
                sendSingle(node, out);
                Serial.printf("Resend to: %u", node);
            }
        }
    }

    const String& getSystemMode(){
        return systemModeState;
    }

    void ignoreConnectionState(const uint32_t& node1, const uint32_t& node2, bool state){
        ignoreNodeState(node1, node2, state);
        ignoreNodeState(node2, node1, state);
    }

    void ignoreNodeState(const uint32_t& node, const uint32_t& ignoredNode, bool state){
        DynamicJsonDocument data(64);

        data["command"] = "ignoreNodeState";
        data["ignoredNode"] = ignoredNode;
        data["state"] = uint8_t(state);

        String out;
        serializeJson(data, out);

        sendSingle(node, out);
    }


};

#endif