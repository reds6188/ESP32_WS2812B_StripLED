#ifndef API_H_
#define API_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include "strip.h"

#define URI_STATUS			"/status"
#define URI_CHRISTMAS		"/christmas"
#define URI_CANDY			"/candy"
#define URI_RAINBOW			"/rainbow"
#define URI_WATER			"/water"
#define URI_TEST			"/test"
#define URI_SET_TEST		"/set-test"
#define URI_NUM_LED			"/num_led"

String api_status(void);
String api_christmas(void);
String api_candy(void);
String api_rainbow(void);
String api_water(void);
String api_test(void);
String api_set_test(uint8_t * payload);
String api_get_num_led(void);
String api_set_num_led(uint8_t * payload);

#endif  /* API_H_ */