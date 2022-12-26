#ifndef EFFECTS_H_
#define EFFECTS_H_

#include <Arduino.h>

#define RED			0xFF0000
#define GREEN		0x00FF00
#define BLUE		0x0000FF
#define YELLOW		0xFFFF00
#define CYAN		0x00FFFF
#define FUCHSIA		0xFF00FF
#define ORANGE      0xFFA500

uint32_t * fillRainbow(uint16_t num);

#endif  /* EFFECTS_H_ */