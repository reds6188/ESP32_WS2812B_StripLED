#include "led_addr.h"

AddrLed::AddrLed(int pin, uint16_t num_of_led) {
    length = num_of_led;
	strip.setPin(pin);
	strip.updateLength(length);
	strip.updateType(NEO_GRB + NEO_KHZ800);
	for(uint16_t i = 0 ; i < length ; i++) {
		buffer[i] = 0;
	}
}

AddrLed::~AddrLed(void) {
    //~strip();
}

void AddrLed::begin(void) {
    strip.begin();
	strip.clear();
    strip.show(); // Initialize all pixels to 'off'
}

void AddrLed::clear(void) {
	for(uint16_t i = 0 ; i < length ; i++) {
		buffer[i] = 0;
	}
	strip.show();
}

void AddrLed::fill(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t color = strip.Color(red,green,blue);
    strip.fill(color, 0, length);
    strip.show();
}

void AddrLed::setPixel(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t color = strip.Color(red,green,blue);
    buffer[pixel] = color;
}

void AddrLed::setPixel(uint16_t pixel, uint32_t color) {
    buffer[pixel] = color;
}

uint32_t AddrLed::getPixel(uint16_t pixel) {
    return buffer[pixel];
}

void AddrLed::setAllPixel(uint32_t *pixel_buffer) {
	for(uint16_t i = 0 ; i < length ; i++) {
		buffer[i] = (*(pixel_buffer + i) & 0x00FFFFFF);
	}
}

void AddrLed::shift(bool left, bool circular) {
    uint32_t temp;
    temp = left ? buffer[0] : buffer[length-1];

    if(left)
    {
        for(uint16_t i = 0 ; i < length-1 ; i++)
            buffer[i] = buffer[i + 1];
        buffer[length-1] = circular ? temp : 0;
    }
    else {
        for(uint16_t i = length-1 ; i > 0 ; i--)
            buffer[i] = buffer[i - 1];
        buffer[0] = circular ? temp : 0;
    }
    //Serial.println("Shift ok");
}

void AddrLed::refresh(void) {
	strip.clear();
	for(uint16_t i = 0 ; i < length ; i++) {
		strip.setPixelColor(i, buffer[i]);
		//Serial.printf("[%i] >> %i -", i, buffer[i]);
	}
	//Serial.println();
	strip.show();
}
