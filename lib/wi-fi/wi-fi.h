#ifndef WI_FI_H_
#define WI_FI_H_

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <console.h>

#define WIFI_T          "WI-FI" // Tag for log

#define WIFI_TIMEOUT    10000   // in milliseconds


class WiFiHandler {
    private:
        char ssid[100];
        char password[100];
		char host_name[64];
        wl_status_t status;
        unsigned long wifi_timeout;
        IPAddress localIP;
    public:
        WiFiHandler(const char* ssid, const char* password);
		void setHostName(const char * name);
        void begin(wifi_mode_t mode);
        void loop(void);
        bool connected(void);
        void scanNetworks(void);
};

/*
void setCredentials(const char* new_ssid, const char* new_pswd);
void scanNetworks(void);
void initWiFi(wifi_mode_t mode);
void checkWiFiConnection(void);
*/

#endif  /* WI_FI_H_ */