#ifndef _SmartObjectMain_
#define _SmartObjectMain_

#include "SmartObjectBasic.hpp"

class SmartObjectMain: public SmartObjectBasic{
protected:

public:

    SmartObjectMain(painlessMesh*  mesh_object):SmartObjectBasic(mesh_object){};

    void systemMode(const String& mode){
        DynamicJsonDocument data(64);

        data["command"] = "systemMode";
        data["mode"] = mode;

        String out;
        serializeJson(data, out);

        mesh->sendBroadcast(out, true);
    }


};

#endif