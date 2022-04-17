#include <Arduino.h>

#ifndef _PIN_CONSTS_
#define _PIN_CONSTS_

    const int redLED = 5; //D3
    const int greenLED = 0; //D1
    const int blueLED = 4; //D2

    const int modeLED = 2; //D4

    const int MAX = 255;

    void setGREEN() {
        analogWrite(redLED, MAX);
        analogWrite(greenLED, 0);
        analogWrite(blueLED, MAX);
    }

    void setRED() {
        analogWrite(redLED, 0);
        analogWrite(greenLED, MAX);
        analogWrite(blueLED, MAX);
    }

    void setBLUE() {
        analogWrite(redLED, MAX);
        analogWrite(greenLED, MAX);
        analogWrite(blueLED, 0);
    }

    void setBLACK() {
        analogWrite(redLED, MAX);
        analogWrite(greenLED, MAX);
        analogWrite(blueLED, MAX);
    }

    void setPURPLE() {
        analogWrite(redLED, 0);
        analogWrite(greenLED, MAX);
        analogWrite(blueLED, 0);
    }

    void setYELLOW() {
        analogWrite(redLED, 0);
        analogWrite(greenLED, 0);
        analogWrite(blueLED, MAX);
    }

    void setAQUA() {
        analogWrite(redLED, MAX);
        analogWrite(greenLED, 0);
        analogWrite(blueLED, 0);
    }

    void setFrogFaints() {
        analogWrite(redLED, MAX - 0x7b);
        analogWrite(greenLED, MAX - 0x91);
        analogWrite(blueLED, MAX - 0x7b);
    }

    void setVeryDeepPurpleRed() {
        analogWrite(redLED, MAX - 0xd7);
        analogWrite(greenLED, MAX - 0x00);
        analogWrite(blueLED, MAX - 0x27);
    }

#endif