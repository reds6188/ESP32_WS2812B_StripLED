#include "effects.h"

uint32_t rainbow[7] = {
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	FUCHSIA,
	RED
};

uint32_t * fillRainbow(uint16_t num) {
    static uint32_t* buff = new uint32_t[num];
	uint16_t div = num / 6;
	uint8_t rem = num % 6;
	uint16_t shades;
	uint8_t cnt = 0;
    uint32_t color, curr, next;

    for(uint8_t i=0 ; i<6 ; i++) {
        shades = (i < rem) ? div + 1 : div;
		color = rainbow[i];
		for(uint8_t j=0 ; j<shades ; j++) {
			 buff[cnt] = color;
			for(uint8_t k=0 ; k<3 ; k++) {
                curr = (uint8_t)(rainbow[i] >> 8*k);
                next = (uint8_t)(rainbow[i+1] >> 8*k);
                if(curr < next)
                    color += ((uint8_t)(255 / shades)) << 8*k;
                else if(curr > next)
                    color -= ((uint8_t)(255 / shades)) << 8*k;
			}
            //Serial.printf("%u >> %x\n", cnt, buff[cnt]);
			cnt++;
		}
	}
	return buff;
}