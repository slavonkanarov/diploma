#ifndef _EFFECTS_
#define _EFFECTS_

#include "Consts.h"
#include "Timer.h"
#include "ColorConverter.h"
#include <Arduino.h>

class Effects {
    private:
        Timer timer;
        bool status = true;
        byte brightness;
        double value;

        void refreshTimer() { timer.start(0); }
        void refreshStatus() { status = true; }
        void refreshBrightness() { brightness = 0; }
        void refreshValue() { value = 1.0; }

        void write(int r, int g, int b) { 
            analogWrite(redLED, MAX - r);
            analogWrite(greenLED, MAX - g);
            analogWrite(blueLED, MAX - b);
        }
    
    public:
        Effects() { timer.start(0); brightness = 0; value = 1.0; }
        ~Effects() {}

        void refresh() {
            refreshBrightness();
            refreshStatus();
            refreshTimer();
            refreshValue();
        }

        /* mode 1 */
        void staticBright(int r, int g, int b) {
            if(timer == 0) {
                write(r,g,b);
                timer.start(1000);
            }
        }

        /* mode 2 */
        void twiling(int r, int g, int b) {
            if(timer == 0) {
                if(status) {
                    write(r,g,b);
                } else {
                    write(0, 0, 0);
                }
                status = !status;
                timer.start(500);
            }
        }
        /* mode 3 */
        void fade(int r, int g, int b) {
            if(timer == 0) {
                double h, s, v = 0;
                ColorConverter::RGBtoHSV(r, g, b, h, s, v);
                if(status) {
                    value += v*(0.02);
                } else {
                    value -= v*(0.02);
                }

                if(value <= 0) {
                    value = 0;
                    status = !status;
                } else if(value >= v) {
                    value = v;
                    status = !status;
                }
                ColorConverter::HSVToRGB(h, s, value, r, g, b);
                write(r, g, b);
                timer.start(100);
            }
        }

        /* mode 4 */
        void rainbow() {
            if(timer == 0) {
                switch (brightness%8) {
                case 0:
                    setVeryDeepPurpleRed();
                    break;
                case 1:
                    setFrogFaints();
                    break;
                case 2:
                    setBLUE();
                    break;
                case 3:
                    setPURPLE();
                    break;
                case 4:
                    setYELLOW();
                    break;
                case 5:
                    setAQUA();
                    break;
                case 6:
                    setRED();
                    break;
                case 7 :
                    setGREEN();
                    break;
                default:
                    break;
                }
                brightness++;
                timer.start(2000);
            }
        }

        /* mode 5 */
        void police() {
             if(timer == 0) {
                switch (brightness%2) {
                case 0:
                    setRED();
                    break;
                case 1:
                    setBLUE();
                    break;
                default:
                    break;
                }
                brightness++;
                timer.start(1000);
            }
        }
};

#endif