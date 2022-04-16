#ifndef _Effects_
#define _Effects_

#include <Arduino.h>
#include <FastLED.h>
#include "LampConsts.h"
#include "Timer.h"
#include "PixelArts.h"
#include "random.h"

class Effects {
protected:
    CRGB leds[NUM_LEDS];
    CFastLED matrix; 
    CRGBPalette16 currentPalette;
    TBlendType currentBlending;
    uint8_t colorU8Index;
    int16_t colorSign16Index;
    Timer colorLoop;
    boolean way;
    Random rand;

public:
    Effects(){
        colorU8Index = 0;
        colorSign16Index = 0;
        matrix.addLeds<CHIPSET, MATRIX_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
        matrix.setBrightness(BRIGHTNESS);
        staticColor(RED, GREEN, BLUE, BRIGHTNESS);
        colorLoop.start(0);
        way = true;
    }
    void refresh(){
            refreshColorIndex();
            refreshColorTimer();
            refreshWay();
            refreshScreen();
    }
    void refreshScreen(){
        for(int i = 0; i < NUM_LEDS; i++){
            leds[i] = CRGB(0, 0, 0);
        }
        matrix.show();
    }
    void refreshWay(){
        way = true;
    }
    void refreshColorIndex(){
        colorSign16Index = 0;
        colorU8Index = 0;
    }
    void refreshColorTimer(){
        colorLoop.start(0);
    }
    /*0 effect*/
    void staticColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness){
        for(int i = 0; i < NUM_LEDS; i++){
            leds[i] = CRGB(red, green, blue);
        }
        matrix.setBrightness(brightness);
        matrix.show();
    }
    /*1 effect*/
    void changeColor(uint8_t brightness){
        currentPalette = RainbowColors_p;
        currentBlending = LINEARBLEND;
        if (colorLoop == 0){
            for(int i = 0; i < NUM_LEDS; i++){
                leds[i] = ColorFromPalette(currentPalette, colorU8Index, brightness, currentBlending);
            }
            colorU8Index += 16;
            colorLoop.start(2500);
        }
        matrix.setBrightness(brightness);
        matrix.show();
    }
    /*2 effect*/
    void rainbowColor(uint8_t brightness){
        currentPalette = RainbowColors_p;
        currentBlending = LINEARBLEND;
        if (colorLoop == 0){
            for(int i = 0; i < NUM_LEDS; i++){
                leds[i] = ColorFromPalette( currentPalette, 
                                            colorU8Index+((uint8_t)(i/4)), 
                                            brightness, 
                                            currentBlending);
            }
            colorU8Index += 4;
            colorLoop.start(50);
        }
        matrix.setBrightness(brightness);
        matrix.show();
    }
    /*3 effect*/
    void twinkleColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness){
        if(brightness == 0) return;
        if(colorLoop == 0){
            for(int i = 0; i < NUM_LEDS; i++){
                leds[i] = CRGB(red, green, blue);
            }
            colorU8Index += (way?1:-1);
            if(colorU8Index >= brightness){
                way = !way;
                colorU8Index = brightness;
            }
            if(colorU8Index <= 0){
                way = !way;
                colorU8Index = 0;
            }
            matrix.setBrightness(colorU8Index);
            colorLoop.start(50);
        }
        matrix.show();
    }
    /*4 && 5 effects*/
    void pullUpCollor(  uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, 
                        uint8_t changing_block_size, float speed){
        
        if(speed > 100) speed = 100;
        if(colorLoop == 0){
            if(colorSign16Index >= NUM_LEDS){
                way = !way;            
                colorSign16Index = NUM_LEDS-changing_block_size;
            }
            if(colorSign16Index < 0){
                way = !way;            
                colorSign16Index = 0;
            }
            if(way == true){
                for(int i = 0; i < changing_block_size; i++){
                    leds[i+colorSign16Index] = CRGB(red, green, blue);     
                }
            } else {
                for(int i = 0; i < changing_block_size; i++){
                    leds[i+colorSign16Index] = CRGB(0, 0, 0);
                }
            }
            colorSign16Index += (way?(changing_block_size):-(changing_block_size));
            colorLoop.start(100 / speed);
        }
        matrix.setBrightness(brightness);
        matrix.show();
    }
    /*6 effect*/
    void HotLineMiami(uint8_t brightness){
        MiamiChicken(leds);
        matrix.setBrightness(brightness);
        matrix.show();
    }
    /*7 && 8 effects*/
    void randomFillByOne(   uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, 
                            float speed, uint8_t colorMode){
        
        if(speed > 100) speed = 100;
        if(colorLoop == 0){
            if(colorU8Index == 0){
                rand.randomize(4, 1);
                way = !way;
                if(colorMode == 0){
                    way?leds[0] = CRGB(0, 0, 0):leds[0] = CRGB(red, green, blue);
                }else if(colorMode == 1){
                    way?leds[0] = CRGB(0, 0, 0):leds[0] = CRGB(rand.randomize(), rand.randomize(), rand.randomize());
                }
            }
            uint8_t index = (rand.getRandom() * colorU8Index)%NUM_LEDS + 1;
            if(colorMode == 0){
                way?leds[index] = CRGB(0, 0, 0):leds[index] = CRGB(red, green, blue);
            } else if(colorMode == 1){
                way?leds[index] = CRGB(0, 0, 0):leds[index] = CRGB(rand.randomize(), rand.randomize(), rand.randomize());
            }
            colorU8Index++;
            colorLoop.start(100/speed);
        }
        matrix.setBrightness(brightness);
        matrix.show();
    }
};

#endif