#ifndef EFFECTS_H_
#define EFFECTS_H_

#include <Arduino.h>
#include <led_addr.h>

#define RED			0xFF0000
#define GREEN		0x00FF00
#define BLUE		0x0000FF
#define YELLOW		0xFFFF00
#define CYAN		0x00FFFF
#define FUCHSIA		0xFF00FF
#define ORANGE      0xFFA500

uint32_t * fillGradient(uint16_t num, uint32_t col_start, uint32_t col_end);
uint32_t * fillRainbow(uint16_t num);
uint32_t changeWaterShade(uint32_t shade, bool changeUp);

#endif  /* EFFECTS_H_ */