#ifndef _SmartPublisher_
#define _SmartPublisher_

#include <Arduino.h>
#include <functional>
#include <string>

using namespace std;

class SmartPublisher{
protected:
    function<void(const SmartPublisher*, const String&)>* fuck;
    String name;
public:

    SmartPublisher(const String& name_of_pub, function<void(const SmartPublisher*, const String&)>& analizer){
        fuck = &analizer;
        name = name_of_pub;
    };

    const String& getName() const{
        return name;
    }

    void publish(const String& event){
        (*fuck)(this, event);
    }

};

#endif