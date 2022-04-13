#ifndef _Scene_
#define _Scene_

#include <Arduino.h>

class Scene{
protected:

public:
    String activator;
    String executor;
    String event;
    uint32_t target;
    Scene(){};

};

#endif