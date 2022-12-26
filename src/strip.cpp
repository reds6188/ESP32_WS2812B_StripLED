#include "strip.h"

stato_strip_t StatoStrip;

unsigned long TimerStrip;
bool direction = false;
bool circular = false;

const uint32_t step = 0x020202;

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
		else
			console.log(WS_T, "WS command not recognized: " + String(*data));
    }
}

void resetStrip(void)
{
    StatoStrip = STRIP_OFF;
	Strip.fillAll(0,0,0);
}

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
				uint8_t num = rand() % 255;
				uint8_t index = rand() % NUM_LED;
				Strip.setPixel(index, num, num, num);
			}
			Strip.refresh();
		}
		else if(StatoStrip == STRIP_RAINBOW)
		{
			Strip.shift(direction, circular);
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