#ifndef _SmartPublisher_
#define _SmartPublisher_

#include <Arduino.h>
#include <functional>
#include <string>

using namespace std;

class SmartActivator{
protected:
    function<void(const SmartActivator*)>* fuck;
    String name;
public:

    SmartActivator(const String& name_of_activator, function<void(const SmartActivator*)>& analizer){
        fuck = &analizer;
        name = name_of_activator;
    };

    const String& getName() const{
        return name;
    }

    void publish(){
        (*fuck)(this);
    }

};

#endif