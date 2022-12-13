#include "rgb_led.h"

RgbLed::RgbLed(rgb_led_t led_pin, led_driver_t led_driver_mode) {
	pin = led_pin;
	driver_mode = led_driver_mode;

	pinMode(pin.r_led, OUTPUT);
	pinMode(pin.g_led, OUTPUT);
	pinMode(pin.b_led, OUTPUT);

	digitalWrite(pin.r_led, (driver_mode == LS_DRIVER ? LOW : HIGH));
	digitalWrite(pin.g_led, (driver_mode == LS_DRIVER ? LOW : HIGH));
	digitalWrite(pin.b_led, (driver_mode == LS_DRIVER ? LOW : HIGH));
}

void RgbLed::setColor(c8_color_t color) {
	bool r = (color & 0x01);
	bool g = ((color >> 1) & 0x01);
	bool b = ((color >> 2) & 0x01);

	digitalWrite(pin.r_led, (driver_mode == LS_DRIVER ? r : !r));
	digitalWrite(pin.g_led, (driver_mode == LS_DRIVER ? g : !g));
	digitalWrite(pin.b_led, (driver_mode == LS_DRIVER ? b : !b));
}

void RgbLed::setBlink(c8_color_t color1, c8_color_t color2, unsigned long time1, unsigned long time2) {
	blink[0].color = color1;
	blink[0].time = time1;
	blink[1].color = color2;
	blink[1].time = time2;

	blink_index = false;
	setColor(color1);
}

void RgbLed::loop(void) {
	
	if((millis() - timer_blink) > blink[blink_index].time) {
		timer_blink = millis();
		blink_index = !blink_index;
		setColor(blink[blink_index].color);
	}
}