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

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWsMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void swReset(void)
{
    console.log(MAIN_T, "Restartin ESP32...");
    ESP.restart();
}

void setup() {
	Serial.begin(115200);
    led.setBlink(C8_YELLOW,C8_BLACK,100,0);
	console.header("START INITIALIZATION", DOUBLE_DASHED, 60);
    wifi_handler.begin(WIFI_STA);
	initWebServer(&htmlProcessor);
	initWebSocket(&onEvent);
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