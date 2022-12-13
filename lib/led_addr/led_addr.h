#ifndef LED_ADDR_H_
#define LED_ADDR_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

//Adafruit_NeoPixel strip(1, -1, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip(num_of_led, pin, NEO_GRB + NEO_KHZ800);

#define NUM_MAX_LED 256

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
}rgb_t;

class AddrLed {
	private:
		Adafruit_NeoPixel strip;
		uint16_t length;
		uint32_t buffer[NUM_MAX_LED];
	public:
		AddrLed(int pin, uint16_t num_of_led);
		~AddrLed(void);
		uint32_t RgbToInt(uint8_t red, uint8_t green, uint8_t blue);
		rgb_t IntToRgb(uint32_t color) ;
		void begin(void);
		void clear(void);
		void fill(uint8_t red, uint8_t green, uint8_t blue);
		void setPixel(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue);
		void setPixel(uint16_t pixel, uint32_t color);
		uint32_t getPixel(uint16_t pixel);
		void setAllPixel(uint32_t *pixel_buffer);
		void shift(bool direction, bool circular);
		void refresh(void);
		uint32_t sumColors(uint32_t color1, uint32_t color2);
		uint32_t diffColors(uint32_t color1, uint32_t color2);
};

#endif	/* LED_ADDR_H_ */