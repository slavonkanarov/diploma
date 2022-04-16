#ifndef _Settings_
#define _Settings_

#include <Arduino.h>
#include "Effects.h"

class Settings {
protected:
    uint8_t mode;
    uint8_t brightness;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    char* print_str;
    Effects effect;

public:
    Settings(){
        print_str = PRINTING_DATA;
        mode = 0;
        brightness = BRIGHTNESS;
        red = RED;
        green = GREEN;
        blue = BLUE;
    }
    boolean checkChanges(uint8_t brightness_, uint8_t red_, uint8_t green_, uint8_t blue_){
        if(     brightness != brightness_ 
            ||  red != red_ 
            ||  green != green_ 
            ||  blue != blue_)
            
            return true;
        else 
            return false;
    }
    boolean checkMode(uint8_t mode_){
        if (mode == mode_)
            return true;
        else 
            return false;
    }
    void setMode(uint8_t mode_){
        if(!checkMode(mode_)){
            effect.refresh();
            mode = mode_;
        }
    }
    void setBrightness(uint8_t brightness_){
        brightness = brightness_;
    }
    void setColor(uint8_t red_, uint8_t green_, uint8_t blue_){
        red = red_;
        green = green_;
        blue = blue_;
    }
    void setAll(uint8_t mode_, uint8_t brightness_, uint8_t red_, uint8_t green_, uint8_t blue_){
        if(checkChanges(brightness_, red_, green_, blue_)){    
            setBrightness(brightness_);
            setColor(red_, green_, blue_);
        }
        setMode(mode_);
    }
    void show(){
        switch (mode)
        {
        case 48:
            effect.staticColor(red, green, blue, brightness);
            break;
        case 49:
            effect.changeColor(brightness);
            break;
        case 50:
            effect.rainbowColor(brightness);
            break;
        case 51:
            effect.twinkleColor(red, green, blue, brightness);
            break;
        case 52:
            effect.pullUpCollor(red, green, blue, brightness, SIZE, 1);
            break;
        case 53:
            effect.pullUpCollor(red, green, blue, brightness, 1, 5);
            break;  
        case 54:
            effect.randomFillByOne(red, green, blue, brightness, 5, 0);
            break;
        case 55:
            effect.randomFillByOne(red, green, blue, brightness, 5, 1);
            break;
        case 56:
            effect.HotLineMiami(brightness);
            break;
        default:
            break;
        }
    }
    ~Settings(){}
};

#endif
