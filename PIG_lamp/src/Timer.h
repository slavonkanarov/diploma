#ifndef _Timer_
#define _Timer_

#include <Arduino.h>

class Timer{
protected:
    unsigned long _lastTime = 0;
    unsigned long _lastTimerStartValue = 0;
    unsigned long _timer = 0;
    bool isWork = true;

    void differenceTimer(){
        removeTime(abs((long long)(millis() - _lastTime)));
        _lastTime = millis();
    }

public:

    Timer(){
        _lastTime = 0;
        _timer = 0;
        _lastTimerStartValue = 0;
        isWork = true;
    };
    Timer(unsigned long time){
        start(time);
    }

    void start(){
        _lastTime = millis();
        isWork = true;
    }

    void start(unsigned long time){
        _lastTimerStartValue = time;
        _lastTime = millis();
        _timer = time;
        isWork = true;
    }

    void restart(){
        start(_lastTimerStartValue);
    }

    unsigned long getStartValue(){
        return _lastTimerStartValue;
    }

    void addTime(unsigned long time){
        _timer += time;
    }

    void removeTime(unsigned long time){
        if(_timer <= time)_timer = 0;
        else _timer -= time;
    }

    void stop(){
        if(isWork)differenceTimer();
        isWork = false;
    }

    unsigned long time(){
        if(isWork)differenceTimer();
        return _timer;
    }

    bool operator==(unsigned long v){
        return this->time() == v;
    }
};

#endif
