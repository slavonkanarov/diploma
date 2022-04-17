#ifndef _COLOR_CONVERTER_
#define _COLOR_CONVERTER_

#include <Arduino.h>

class ColorConverter {
    public:
        static double threeway_max(double a, double b, double c) {
	        return max(a, max(b, c));
        }

        static double threeway_min(double a, double b, double c) {
	        return min(a, min(b, c));
        }

        // void getHSV(double& hue, double& saturation, double& value) {
        //     hue = h;
        //     saturation = s;
        //     value = v;
        // }

        static void RGBtoHSV(int r, int g, int b, double& h, double& s, double& v) {
            double rd = double(r) / 255;
            double gd = double(g) / 255;
            double bd = double(b) / 255;
            double max = threeway_max(rd, gd, bd), min = threeway_min(rd, gd, bd);
            
            v = max;

            double d = max - min;
            s = max == 0 ? 0 : d / max;

            h = 0;
            if (max != min) {
                if (max == rd) {
                    h = (gd - bd) / d + (gd < bd ? 6 : 0);
                } else if (max == gd) {
                    h = (bd - rd) / d + 2;
                } else if (max == bd) {
                    h = (rd - gd) / d + 4;
                }
                h /= 6;
            }
        }

        static void HSVToRGB(double hue, double saturation, double value, int& red, int& green, int& blue) {
            double r = 0;
            double g = 0;
            double b = 0;

            int i = int(hue * 6);
            auto f = hue * 6 - i;
            auto p = value * (1 - saturation);
            auto q = value * (1 - f * saturation);
            auto t = value * (1 - (1 - f) * saturation);

            switch (i % 6) {
            case 0: r = value , g = t , b = p;
                break;
            case 1: r = q , g = value , b = p;
                break;
            case 2: r = p , g = value , b = t;
                break;
            case 3: r = p , g = q , b = value;
                break;
            case 4: r = t , g = p , b = value;
                break;
            case 5: r = value , g = p , b = q;
                break;
            }

            red = int(r * 255);
            green = int(g * 255);
            blue = int(b * 255);
        }
};

#endif