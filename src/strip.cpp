#include "strip.h"

Preferences store;
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

void setNumLed(uint16_t num) {
	uint16_t currLength = Strip.getLength();
	if(currLength != num)
	{
		store.begin("store", false);	// Read/Write mode
		store.putUShort("num_of_leds", num);
		Strip.setLength(num);
		store.end();
	}
	resetStrip();
	led.setBlink(C8_GREEN,C8_BLACK,500,500);
}

/*
void handleWsMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
	StaticJsonDocument<64> rxJson;
    String wsRes;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;

		DeserializationError err = deserializeJson(rxJson, data);
		JsonObject rxJsonObj = rxJson.as<JsonObject>();

		if(!err)
		{
			String cmd = rxJsonObj["action"];

			if(cmd.equals("set-num-led"))
			{
				uint16_t val = rxJsonObj["value"];
				uint16_t currLength = Strip.getLength();
				console.log(WS_T, "'set-num-led' command received");
				offStrip();
				if(currLength != val)
				{
					store.begin("store", false);	// Read/Write mode
					store.putUShort("num_of_leds", val);
					Strip.setLength(val);
					store.end();
				}
				wsUpdateMsg("set-num-led", 0);
				resetStrip();
				led.setBlink(C8_GREEN,C8_BLACK,500,500);
			}
			else if(cmd.equals("get-num-led"))
			{
				uint16_t currLength = Strip.getLength();
				console.log(WS_T, "'get-num-led' command received");
				wsUpdateMsg("get-num-led", currLength);
			}
			else if(cmd.equals("off"))
			{
				console.log(WS_T, "'off' command received");
				offStrip();
				wsUpdateMsg("off", 0);
				led.setBlink(C8_GREEN,C8_BLACK,500,500);
			}
			else if(cmd.equals("christmas"))
			{
				console.log(WS_T, "'christmas' command received");
				if(StatoStrip != STRIP_OFF)
					offStrip();
				StatoStrip = STRIP_CHRISTMAS;
				wsUpdateMsg("christmas", 0);
				led.setBlink(C8_BLUE,C8_BLACK,500,500);
			}
			else if(cmd.equals("rainbow"))
			{
				console.log(WS_T, "'rainbow' command received");
				if(StatoStrip != STRIP_OFF)
					offStrip();
				StatoStrip = STRIP_RAINBOW;
				startRainbow();
				wsUpdateMsg("rainbow", 0);
				led.setBlink(C8_FUCHSIA,C8_BLACK,500,500);
			}
			else if(cmd.equals("water"))
			{
				console.log(WS_T, "'water' command received");
				if(StatoStrip != STRIP_OFF)
					offStrip();
				StatoStrip = STRIP_WATER;
				startWater();
				wsUpdateMsg("water", 0);
				led.setBlink(C8_CYAN,C8_BLACK,500,500);
			}
			else
				console.log(WS_T, "WS command not recognized: " + String(*data));
		}
		else
			console.log(WS_T, "Unkown JSON content");
    }
}
	*/

void clearStrip(void) {
	Strip.fillAll(0,0,0);
}

void offStrip(void)
{
	StatoStrip = STRIP_OFF;
	Strip.fillAll(0,0,0);
}

void resetStrip(void)
{
	offStrip();
	store.begin("store", true);	// Read mode
    uint16_t numOfLeds = store.getUShort("num_of_leds", NUM_LED);
	Strip.setLength(numOfLeds);
	store.end();
}

void testStrip(void)
{
	uint16_t stripLength = Strip.getLength();
	for(uint16_t i=0 ; i<stripLength ; i++) {
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

void fRainbow(void)
{
	if(StatoStrip != STRIP_OFF)
	{
		offStrip();
		led.setBlink(C8_GREEN,C8_BLACK,500,500);
	}
	else
	{
		startRainbow();
		StatoStrip = STRIP_RAINBOW;
		led.setBlink(C8_FUCHSIA,C8_BLACK,500,500);
	}
}

/*
#define WW_R	255
#define WW_G	236
#define WW_B	214
*/
#define WW_R	255
#define WW_G	192//236
#define WW_B	32//48

int chr_step = 50;
float chr_gamma = 2.2f;
float linearT = 1.0f - ((float)1 / chr_step);
float t_gamma = pow(linearT, chr_gamma);
rgb_t gamma_rgb = {
	.r = (uint8_t)WW_R * t_gamma,
	.g = (uint8_t)WW_G * t_gamma,
	.b = (uint8_t)WW_B * t_gamma
};
int test_index;
int test_num_step = 50;
//const uint32_t step2 = 0x0C0801;
const uint32_t step2 = 0x0C0601;


//const uint32_t test_step = 0x040401;
//const uint32_t test_step = 0x80802;
//const uint32_t test_step = 0x0F0802;

rgb_t test_seed = {
	.r = 255,
	.g = 255,
	.b = 255
};

rgb_t test_step = {
	.r = 5,
	.g = 5,
	.b = 5
};

void startTest(void) {
	//Strip.setPixel(0, WW_R, WW_G, WW_B);
	//rgb_t test_rgb = Strip.IntToRgb(test_step);
	Strip.setPixel(0, test_seed);
	console.log(STRIP_T, "Seed >>> " + String(test_seed.r) + " - " + String(test_seed.g) + " - " + String(test_seed.b));
	console.log(STRIP_T, "Step >>> " + String(test_step.r) + " - " + String(test_step.g) + " - " + String(test_step.b));
	test_index = 0;
}

void loopStrip(void)
{
	if(StatoStrip == STRIP_TEST) {
		for(uint8_t i=0 ; i<22 ; i++) {
			//Strip.setPixel(i, WW_R, WW_G, WW_B);
			Strip.setPixel(i, test_step.r * i, test_step.g * i, test_step.b * i);
			rgb_t test_rgb = Strip.IntToRgb(Strip.getPixel(i)); 
			console.log(STRIP_T, String(i) + " >>> " + String(test_rgb.r) + " - " + String(test_rgb.g) + " - " + String(test_rgb.b));
			/*
			for(uint8_t j=0 ; j<i ; j++)
				Strip.removeColorToPixel(i, test_step);
			rgb_t test_rgb = Strip.IntToRgb(Strip.getPixel(i)); 
			console.log(STRIP_T, String(i) + " >>> " + String(test_rgb.r) + " - " + String(test_rgb.g) + " - " + String(test_rgb.b));
			*/
		}
		Strip.refresh();
		StatoStrip = STRIP_OFF;
		/*
		if(millis() - TimerStrip > 100)
		{
			TimerStrip = millis();
			if(!test_index) {
				Strip.setPixel(0, WW_R, WW_G, WW_B);
			}
			else {
				Strip.removeColorToPixel(0, test_step);
			}
			rgb_t test_rgb = Strip.IntToRgb(Strip.getPixel(0)); 
			console.log(STRIP_T, String(test_index) + " >>> " + String(test_rgb.r) + " - " + String(test_rgb.g) + " - " + String(test_rgb.b));
			if(test_index < test_num_step)
				test_index++;
			else {
				test_index = 0;
				StatoStrip = STRIP_OFF;
			}
			Strip.refresh();
		}
		*/
	}
	else if(millis() - TimerStrip > TIME_REFRESH)
	{
		TimerStrip = millis();

		if(StatoStrip == STRIP_CHRISTMAS)
		{
			uint16_t stripLength = Strip.getLength();

			for(uint16_t i=0 ; i<stripLength ; i++) {
				//Strip.removeColorToPixel(i, gamma_rgb);
				Strip.removeColorToPixel(i, step2);
			}
			uint8_t ignite = rand() % 2;
			if(ignite == 0) {
				uint8_t index = rand() % stripLength;
				//--------------------------------------
				//uint8_t num = rand() % 255;
				//Strip.setPixel(index, num, num, num);
				//Strip.setPixel(index, WW_R, WW_G, WW_B);

				//Strip.setPixel(index, 252, 168, 21);
				Strip.setPixel(index, 252, 126, 21);
				//--------------------------------------
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
	uint16_t stripLength = Strip.getLength();
	uint32_t *led_buffer;
	led_buffer = fillRainbow(stripLength);
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
