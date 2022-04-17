#ifndef _SETTINGS_
#define _SETTINGS_

#include "Effects.h"

class Settings {
    private:
        int red;
        int green;
        int blue;
        int mode;

        Effects effect;
    public:
        Settings() { red = 0; green = 0; blue = 0; mode = 1; }
        ~Settings() {}
        
        void setMode(int new_mode) { mode = new_mode; }
        void setRed(int new_red) { red = new_red; }
        void setGreen(int new_green) { green = new_green; }
        void setBlue(int new_blue) { blue = new_blue; }
        void setRGB(int r, int g, int b) { setRed(r); setGreen(g); setBlue(b); }
        void setAll(int r, int g, int b, int m) { 
            if(checkChange(r, g, b, m)) {
                effect.refresh();
            }
            setRGB(r, g, b); setMode(m);
        }
        bool checkChange(int r, int g, int b, int m) {
            // return (red != r || green != g || blue != b || mode != m);
            return (mode != m);
        }
        void show() {
            switch (mode) {
            case 1:
                effect.staticBright(red, green, blue);
                break;
            case 2:
                effect.twiling(red, green, blue);
                break;
            case 3:
                effect.fade(red, green, blue);
                break;
            case 4:
                effect.rainbow();
                break;
            case 5:
                effect.police();
                break;
            default:
                effect.staticBright(MAX, MAX, MAX);
            }
        }
};

#endif