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

void AddrLed::setLength(uint16_t num_of_led) {
    length = num_of_led;
	strip.updateLength(length);
    	for(uint16_t i = 0 ; i < length ; i++) {
		buffer[i] = 0;
	}
}

uint16_t AddrLed::getLength(void) {
    return length;
}

uint32_t AddrLed::RgbToInt(uint8_t red, uint8_t green, uint8_t blue) {
    return strip.Color(red, green, blue);
}

uint32_t AddrLed::RgbToInt(rgb_t rgb) {
    return strip.Color(rgb.r, rgb.g, rgb.b);
}

rgb_t AddrLed::IntToRgb(uint32_t color) {
    rgb_t rgbColor;

    rgbColor.r = (uint8_t)(color >> 16);
    rgbColor.g = (uint8_t)(color >> 8);
    rgbColor.b = (uint8_t)(color);

    return rgbColor;
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

void AddrLed::fill(uint8_t red, uint8_t green, uint8_t blue, uint16_t length) {
    uint32_t color = strip.Color(red,green,blue);
    strip.fill(color, 0, length);
    for(uint16_t i = 0 ; i < length ; i++) {
		buffer[i] = color;
	}
    strip.show();
}

void AddrLed::fill(rgb_t color, uint16_t length) {
    uint32_t ui32_color = strip.Color(color.r, color.g, color.b);
    strip.fill(ui32_color, 0, length);
    for(uint16_t i = 0 ; i < length ; i++) {
		buffer[i] = ui32_color;
	}
    strip.show();
}


void AddrLed::fillAll(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t color = strip.Color(red,green,blue);
    strip.fill(color, 0, length);
    for(uint16_t i = 0 ; i < length ; i++) {
		buffer[i] = color;
	}
    strip.show();
}

void AddrLed::fillAll(rgb_t color) {
    uint32_t ui32_color = strip.Color(color.r, color.g, color.b);
    strip.fill(ui32_color, 0, length);
    for(uint16_t i = 0 ; i < length ; i++) {
		buffer[i] = ui32_color;
	}
    strip.show();
}

void AddrLed::setPixel(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t color = strip.Color(red,green,blue);
    buffer[pixel] = color;
}

void AddrLed::setPixel(uint16_t pixel, uint32_t color) {
    buffer[pixel] = color;
}

void AddrLed::setPixel(uint16_t pixel, rgb_t color) {
    buffer[pixel] = RgbToInt(color);
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

// Add color to current pixel color -----------------------------------------------------
void AddrLed::addColorToPixel(uint16_t pixel, uint32_t color) {
    rgb_t rgb_color = IntToRgb(color);
    rgb_t pixel_color = IntToRgb(getPixel(pixel));
    uint32_t res_color = sumColors(rgb_color, pixel_color);
    setPixel(pixel, res_color);
    //strip.setPixelColor(pixel, res_color);
}

// Remove color to current pixel color --------------------------------------------------
void AddrLed::removeColorToPixel(uint16_t pixel, uint32_t color) {
    rgb_t rgb_color = IntToRgb(color);
    rgb_t pixel_color = IntToRgb(getPixel(pixel));
    uint32_t res_color = diffColors(pixel_color, rgb_color);
    setPixel(pixel, res_color);
    //strip.setPixelColor(pixel, res_color);
}

// Remove color to current pixel color --------------------------------------------------
void AddrLed::removeColorToPixel(uint16_t pixel, rgb_t color) {
    rgb_t pixel_color = IntToRgb(getPixel(pixel));
    uint32_t res_color = diffColors(pixel_color, color);
    setPixel(pixel, res_color);
    //strip.setPixelColor(pixel, res_color);
}

// Returns the sum between uint32-type color1 and color2 --------------------------------
uint32_t AddrLed::sumColors(uint32_t color1, uint32_t color2) {
    uint8_t r1,g1,b1;
    uint8_t r2,g2,b2;

    r1 = (uint8_t)(color1 >> 16),
    g1 = (uint8_t)(color1 >>  8),
    b1 = (uint8_t)(color1 >>  0);

    r2 = (uint8_t)(color2 >> 16),
    g2 = (uint8_t)(color2 >>  8),
    b2 = (uint8_t)(color2 >>  0);

    return strip.Color(constrain(r1+r2, 0, 255), constrain(g1+g2, 0, 255), constrain(b1+b2, 0, 255));
}

// Returns the sum between rgb-type color1 and color2 -----------------------------------
uint32_t AddrLed::sumColors(rgb_t color1, rgb_t color2) {
    uint8_t r = constrain(color1.r + color2.r, 0, 255);
    uint8_t g = constrain(color1.g + color2.g, 0, 255);
    uint8_t b = constrain(color1.b + color2.b, 0, 255);

    return strip.Color(r, g, b);
}

// Returns the difference between uint32-type color1 and color2 -------------------------
uint32_t AddrLed::diffColors(uint32_t color1, uint32_t color2) {
    int16_t r,g,b;
    uint8_t r1,g1,b1;
    uint8_t r2,g2,b2;

    r1 = (uint8_t)(color1 >> 16),
    g1 = (uint8_t)(color1 >>  8),
    b1 = (uint8_t)(color1 >>  0);

    r2 = (uint8_t)(color2 >> 16),
    g2 = (uint8_t)(color2 >>  8),
    b2 = (uint8_t)(color2 >>  0);

    r = (int16_t)r1 - (int16_t)r2;
    g = (int16_t)g1 - (int16_t)g2;
    b = (int16_t)b1 - (int16_t)b2;

    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;

    return strip.Color(r,g,b);
}

// Returns the difference between rgb-type color1 and color2 ----------------------------
uint32_t AddrLed::diffColors(rgb_t color1, rgb_t color2) {
    int16_t r = (int16_t)color1.r - (int16_t)color2.r;
    int16_t g = (int16_t)color1.g - (int16_t)color2.g;
    int16_t b = (int16_t)color1.b - (int16_t)color2.b;

    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;

    return strip.Color(r,g,b);
}