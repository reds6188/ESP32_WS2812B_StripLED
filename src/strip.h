#ifndef STRIP_H_
#define STRIP_H_

#include <Arduino.h>
#include <console.h>
#include <rgb_led.h>
#include <led_addr.h>
#include <web-server.h>
#include "main.h"
#include "effects.h"

#define STRIP_LED       12
#define NUM_LED         50
#define TIME_REFRESH    20  // in ms

typedef enum {
	STRIP_OFF,
	STRIP_CHRISTMAS,
	STRIP_RAINBOW,
}stato_strip_t;

extern stato_strip_t StatoStrip;

void resetStrip(void);
void loopStrip(void);
String htmlProcessor(const String& var);
void handleWsMessage(void *arg, uint8_t *data, size_t len);
void startRainbow(void);

#endif  /* STRIP_H_ */