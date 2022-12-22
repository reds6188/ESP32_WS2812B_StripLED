#include <Arduino.h>
#include <console.h>
#include <rgb_led.h>
#include <led_addr.h>
#include <wi-fi.h>
#include <web-server.h>

#define STRIP_LED       12
#define NUM_LED         50

#define TIME_REFRESH    20  // in ms