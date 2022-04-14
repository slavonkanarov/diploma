#ifndef _SmartValue_
#define _SmartValue_

#include <Arduino.h>
#include <functional>
#include <string>

using namespace std;

class SmartValue{
protected:
    function<void(const String&)> parseValue;
    function<void(const String&, String&)> parseEvent;
    String name;
    String value;
public:

    SmartValue(const String& name_of_value, 
                const function<void(const String&, String&)>& parseEvent_setNewValue,
                const function<void(const String&)>& processValue_doSometing){
        parseEvent = parseEvent_setNewValue;
        parseValue = processValue_doSometing;
        name = name_of_value;
    };

    void processEvent(const String& event){
        parseEvent(event, this->value);
        processValue();
    }

    void processValue(){
        parseValue(this->getValue());
    }

    void setValue(const String& new_value){
        value = new_value;
        processValue();
    }

    const String& getValue() const{
        return value;
    }

    const String& getName() const{
        return name;
    }

};

#endif