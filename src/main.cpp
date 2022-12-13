#include "main.h"

AddrLed Strip(STRIP_LED, NUM_LED);

unsigned long TimerIdle;

void setup() {
	Serial.begin(115200);
	Serial.println("START!!!");
	Strip.setPixel(49, 255, 255, 255);
	Strip.refresh();
}

const uint8_t step = 5;

void loop() {
	if(millis() - TimerIdle > TIME_REFRESH) {
		TimerIdle = millis();
		for(uint16_t i=0 ; i<NUM_LED ; i++) {
			uint32_t color = Strip.getPixel(i);
			uint8_t r = (uint8_t)(color >> 16);
			uint8_t g = (uint8_t)(color >> 8);
			uint8_t b = (uint8_t)(color);
			Strip.setPixel(i, r-step, g-step, b-step);
		}
		uint8_t ignite = rand() % 4;
		if(ignite == 0) {
			uint8_t num = rand() % 255;
			uint8_t index = rand() % NUM_LED;
			Strip.setPixel(index, num, num, num);
		}
		Strip.refresh();
	}
}