#include <Arduino.h>

class HexConverter{
public:
    static byte hexParser(byte hex){
        if(hex >= 48 && hex <= 57){
            return hex%48;
        } else if(hex >= 97 && hex <= 102){
            return (hex%97) + 10;
        }
        return hex;
    }
    static uint8_t hexToUInt8_t(byte highByte, byte lowByte){
        return ((highByte * 16) + (lowByte));
    } 
};

