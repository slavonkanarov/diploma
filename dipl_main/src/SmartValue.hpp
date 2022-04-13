#ifndef _SmartValue_
#define _SmartValue_

#include <Arduino.h>
#include <functional>
#include <string>

using namespace std;

class SmartValue{
protected:
    function<void(const String&)> func;
    String name;
    String value;
    bool sig;
public:

    SmartValue(const String& name_of_device, 
                const function<void(const String&)>& f){
        func = f;
        name = name_of_device;
    };

    void processEvent(const String& event){
        func(event);
    }

    const String& getValue() const{
        return value;
    }

    const String& getName() const{
        return name;
    }

};

#endif