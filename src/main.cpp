#include "main.h"

AddrLed Strip(STRIP_LED, NUM_LED);

rgb_led_t rgb_pin = {
	.r_led = 27,
	.g_led = 14,
	.b_led = 26
};

RgbLed led(rgb_pin, LS_DRIVER);

unsigned long TimerIdle;

void setup() {
	Serial.begin(115200);
	console.header("START INITIALIZATION", DOUBLE_DASHED, 60);

	initWiFi(WIFI_STA);

	//Strip.fill(255, 255, 255, 20);
	Strip.refresh();

	led.setBlink(C8_GREEN,C8_BLACK,500,500);

	console.header("END INITIALIZATION", DOUBLE_DASHED, 60);
}

const uint32_t step = 0x050505;

void loop() {
	led.loop();

	if(millis() - TimerIdle > TIME_REFRESH) {
		TimerIdle = millis();
		for(uint16_t i=0 ; i<NUM_LED ; i++) {
			Strip.removeColorToPixel(i, step);
		}
		uint8_t ignite = rand() % 2;
		if(ignite == 0) {
			uint8_t num = rand() % 255;
			uint8_t index = rand() % NUM_LED;
			Strip.setPixel(index, num, num, num);
		}
		Strip.refresh();
	}
}