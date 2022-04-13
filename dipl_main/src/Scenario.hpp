#ifndef _Scenario_
#define _Scenario_

#include <Arduino.h>

class Scenario{
protected:

public:
    String activatedPublisherName;
    String activatedEvent;
    String executorValueName;
    String event;
    uint32_t target;
    Scenario(){};

};

#endif