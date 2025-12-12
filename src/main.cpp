#include "main.h"

WiFiHandler wifi_handler(wifi_ssid, wifi_password);

Timer TimerIdle;

rgb_led_t rgb_pin = {
	.r_led = RED_LED,
	.g_led = GRN_LED,
	.b_led = BLU_LED
};

Button btn1(BUTTON_1, 80);
Button btn2(BUTTON_2, 80);
RgbLed led(rgb_pin, LS_DRIVER);

void swReset(void)
{
    console.log(MAIN_T, "Restartin ESP32...");
    ESP.restart();
}

void setup() {
	Serial.begin(115200);
    led.setBlink(C8_YELLOW,C8_BLACK,100,0);
	console.header("START INITIALIZATION", DOUBLE_DASHED, 60);
	wifi_handler.setHostName("test");
    wifi_handler.begin(WIFI_STA);
	startWebServer();
	addGetCallback(URI_STATUS, api_status);
	addGetCallback(URI_CHRISTMAS, api_christmas);
	addGetCallback(URI_CANDY, api_candy);
	addGetCallback(URI_RAINBOW, api_rainbow);
	addGetCallback(URI_WATER, api_water);
	addGetCallback(URI_TEST, api_test);
	addPostCallback(URI_SET_TEST, api_set_test);
	addGetCallback(URI_NUM_LED, api_get_num_led);
	addPostCallback(URI_NUM_LED, api_set_num_led);
    btn1.onPress(fRainbow);
    btn2.onPress(swReset);
	resetStrip();
	led.setBlink(C8_GREEN,C8_BLACK,500,500);
	console.header("END INITIALIZATION", DOUBLE_DASHED, 60);
}

void loop() {
	led.loop();
    btn1.loop();
    btn2.loop();
	loopStrip();
    if(TimerIdle.elapsedX1ms(1000)) {
        TimerIdle.trigger();
        wifi_handler.loop();
    }
}