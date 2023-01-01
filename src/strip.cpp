#include "strip.h"

stato_strip_t StatoStrip;

unsigned long TimerStrip;
bool direction = false;
bool circular = false;

const uint32_t step = 0x020201;
uint32_t currPixel;

AddrLed Strip(STRIP_LED, NUM_LED);

String htmlProcessor(const String& var) {
    console.log(MAIN_T, "var = " + var);
    String strProc = String();
	
	switch(StatoStrip)
	{
		case STRIP_OFF:
			//strProc = (var == "F_BTN_STYLE") ? "grn" : "blu";
			strProc = "blu";
			break;
		case STRIP_CHRISTMAS:
			strProc = (var == "CT_BTN_STYLE") ? "grn" : "blu";
			break;
		case STRIP_RAINBOW:
			strProc = (var == "RB_BTN_STYLE") ? "grn" : "blu";
			break;
		case STRIP_WATER:
			strProc = (var == "WT_BTN_STYLE") ? "grn" : "blu";
			break;
		default:
			strProc = String();
			break;
	}

    console.log(MAIN_T, "strProc = " + strProc);
	return strProc;
}

void handleWsMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    String wsRes;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        if(!strcmp((char*)data, "off"))
        {
            console.log(WS_T, "'off' command received");
			resetStrip();
			StatoStrip = STRIP_OFF;
            wsUpdateMsg("off");
			led.setBlink(C8_GREEN,C8_BLACK,500,500);
        }
        else if(!strcmp((char*)data, "christmas"))
        {
            console.log(WS_T, "'christmas' command received");
			if(StatoStrip != STRIP_OFF)
				resetStrip();
			StatoStrip = STRIP_CHRISTMAS;
            wsUpdateMsg("christmas");
			led.setBlink(C8_BLUE,C8_BLACK,500,500);
        }
		else if(!strcmp((char*)data, "rainbow"))
        {
            console.log(WS_T, "'rainbow' command received");
			if(StatoStrip != STRIP_OFF)
				resetStrip();
			StatoStrip = STRIP_RAINBOW;
			startRainbow();
            wsUpdateMsg("rainbow");
			led.setBlink(C8_FUCHSIA,C8_BLACK,500,500);
        }
		else if(!strcmp((char*)data, "water"))
        {
            console.log(WS_T, "'water' command received");
			if(StatoStrip != STRIP_OFF)
				resetStrip();
			StatoStrip = STRIP_WATER;
			startWater();
            wsUpdateMsg("water");
			led.setBlink(C8_CYAN,C8_BLACK,500,500);
        }
		else
			console.log(WS_T, "WS command not recognized: " + String(*data));
    }
}

void resetStrip(void)
{
    StatoStrip = STRIP_OFF;
	Strip.fillAll(0,0,0);
}

void testStrip(void)
{
	for(uint16_t i=0 ; i<NUM_LED ; i++) {
		/*
		uint8_t r = 250-i*4;
		uint8_t g = 250-i*8;
		uint8_t b = 250-i*16;
		*/
		uint8_t r = 200-i*8;//4;
		uint8_t g = 150-i*8;
		uint8_t b = i*4 > 44 ? 6 : 50-i*4;
		console.log(STRIP_T, String(i) + " >>> " + String(r) + " - " + String(g) + " - " + String(b));
		Strip.setPixel(i, r, g, b);
	}
	Strip.refresh();
}

bool flg;

void loopStrip(void)
{
	if(millis() - TimerStrip > TIME_REFRESH)
	{
		TimerStrip = millis();

		if(StatoStrip == STRIP_CHRISTMAS)
		{
			for(uint16_t i=0 ; i<NUM_LED ; i++) {
				Strip.removeColorToPixel(i, step);
			}
			uint8_t ignite = rand() % 2;
			if(ignite == 0) {
				uint8_t index = rand() % NUM_LED;
				uint8_t num = rand() % 255;
				Strip.setPixel(index, num, num, num);
			}
			Strip.refresh();
		}
		else if(StatoStrip == STRIP_RAINBOW)
		{
			Strip.shift(direction, circular);
			Strip.refresh();
		}
		else if(StatoStrip == STRIP_WATER)
		{
			currPixel = Strip.getPixel(0);		// Get first pixel color
			Strip.shift(direction, circular);	// Shift all pixels
			
			uint8_t change = rand() % 100;
			if(change == 0 || (currPixel == 0x000005) || (currPixel == 0xFFFFFF))
				flg = !flg;

			uint32_t col = changeWaterShade(currPixel, flg);
			Strip.setPixel(0, col);

			Strip.refresh();
		}
	}
}

void startRainbow(void)
{
	uint32_t *led_buffer;
	led_buffer = fillRainbow(NUM_LED);
	Strip.setAllPixel(led_buffer);
	direction = false;
	circular = true;
}

void startWater(void)
{
	uint8_t intensity = rand() % 255;
	direction = false;
	circular = false;
	Strip.setPixel(0, 0, 0, intensity);
}


void startGradient(void)
{
	uint32_t *led_buffer;
	led_buffer = fillGradient(10, 0xFFFFFF, 0x00FFFF);
	Strip.setAllPixel(led_buffer);
	Strip.refresh();
	direction = false;
	circular = true;
}
