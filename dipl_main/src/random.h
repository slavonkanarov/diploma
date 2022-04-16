#include<Arduino.h>
#include"LampConsts.h"

class Random {
protected:
	uint8_t rand;
public:
	Random(){
        randomSeed(analogRead(0));
		rand = 0;
	}
	uint8_t randomize(uint8_t module, uint8_t element_for_build_GF){
		rand = (uint8_t)random(1, NUM_LEDS);
		while(rand%module != element_for_build_GF) {
			rand++;
		}	
		return rand;
	}
	uint8_t randomize(){
		return (uint8_t)random(0, NUM_LEDS);
		
	}
	uint8_t getRandom(){
		return rand;
	}
};
