#ifndef WI_FI_H_
#define WI_FI_H_

#include <Arduino.h>
#include <WiFi.h>
#include <console.h>

#define WIFI_TIMEOUT    10000   // in milliseconds

extern void initWiFi(wifi_mode_t mode);

#endif  /* WI_FI_H_ */