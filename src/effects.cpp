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

const uint8_t wave = 15;

uint32_t * fillGradient(uint16_t num, uint32_t col_start, uint32_t col_end) {
	static uint32_t* buff = new uint32_t[num];
	uint8_t g_step[3];
	uint8_t g_rem[3];
	bool g_sign[3];

	// Calculating gradient color step for each component (R, G, B)
	for(uint8_t i=0 ; i<3 ; i++)
	{
		uint8_t c_start = col_start >> i*8;
		uint8_t c_end = col_end >> i*8;
		if(c_start > c_end)
		{
			g_step[i] = c_start - c_end;
			g_sign[i] = false;
		}
		else
		{
			g_step[i] = c_end - c_start;
			g_sign[i] = true;
		}
		
		g_rem[i] = g_step[i] % (num - 1);
		g_step[i] /= (num - 1);
		Serial.println(g_step[i]);
	}

	buff[0] = col_start;
	for(uint16_t i=1 ; i<num ; i++)
	{
		buff[i] = 0;
		Serial.printf("%d >>", i); 
		for(uint8_t j=0 ; j<3 ; j++)
		{
			uint8_t comp = (buff[i-1] >> j*8);
			if(g_sign[j] == true)
				comp += g_step[j];
			else
				comp -= g_step[j];
			buff[i] |= comp << j*8;
			Serial.printf("%d - ", comp); 
		}
		Serial.println();
		
	}

	return buff;
}

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

uint32_t changeWaterShade(uint32_t shade, bool changeUp)
{
	rgb_t col = {
		.r = (shade >> 16),
		.g = (shade >> 8),
		.b = (shade),
	};

	if(changeUp)
	{
		if(col.b < (255 - wave))
			col.b += wave;
		else if(col.g < (255 - wave))
		{
			col.b = 255;
			col.g += wave;
		}
		else if(col.r < (255 - wave))
		{
			col.b = 255;
			col.g = 255;
			col.r += wave;
		}
		else
		{
			col.b = 255;
			col.g = 255;
			col.r = 255;	
		}
	}
	else
	{
		if(col.r > wave)
			col.r -= wave;
		else if(col.g > wave)
		{
			col.r = 0;
			col.g -= wave;
		}
		else if(col.b > wave)
		{
			col.r = 0;
			col.g = 0;
			col.b -= wave;	
		}	
		else
		{
			col.r = 0;
			col.g = 0;
			col.b = 5;	
		}
	}

	return (col.r << 16) | (col.g << 8) | col.b;
}