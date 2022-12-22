#include "wi-fi.h"
#include <ESPmDNS.h>

// Replace with your network credentials
/*
const char* ssid     = "FanHeaterV1";
const char* password = "123456789";
*/
/*
const char* ssid     = "FAIRWIND_WIFI";
const char* password = "Fair2020";
*/

const char* ssid     = "OPPO A16s";
const char* password = "t362mi9s";

long start_wifi_millis;

bool checkConnection(void)
{
    start_wifi_millis = millis();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        //Serial.print(".");
        console.write(WIFI_T, ".");
        if(millis() - start_wifi_millis > WIFI_TIMEOUT)
        {
            WiFi.disconnect(true, true);
            //Serial.println();
            return false;
        }
    }
    //Serial.println();
    return true;
}

void initWiFi(wifi_mode_t mode)
{
    console.log(WIFI_T, "Starting Wi-Fi configuration...");
    if(mode == WIFI_AP)
    {
        IPAddress localIP(192, 168, 4, 1);    // Set your Static IP address
        IPAddress gatewayAddr(192, 168, 4, 0);  // Set your Gateway IP address
        IPAddress subnetMask(255, 255, 255, 0);   // Set your Subnet Mask

        console.log(WIFI_T, "Setting AP (Access Point)...");
        WiFi.mode(WIFI_AP);

        if(!WiFi.softAP(ssid, password))
        {
            console.log(WIFI_T, "AP failed to start");
        }
        delay(100);
        if(!WiFi.softAPConfig(localIP, gatewayAddr, subnetMask))
        {
            console.log(WIFI_T, "AP configuration failed");
        }
        else
        {
            console.log(WIFI_T, "AP started!");
            console.log(WIFI_T, "Network SSID: " + String(ssid));
            IPAddress IP = WiFi.softAPIP();
            console.log(WIFI_T, "AP IP address: " + IP.toString());
        }
    }
    else if(mode == WIFI_STA)
    {
        console.log(WIFI_T, "Setting STA (STAtion mode)...");
        WiFi.mode(WIFI_STA);
        while(!checkConnection())
        {
            console.log(WIFI_T, "Error: failed to connect.");
            console.log(WIFI_T, "Connection restart...");
        }
        console.log(WIFI_T, "Wi-Fi connected!");
        console.log(WIFI_T, "Network SSID: " + String(ssid));
        IPAddress IP = WiFi.localIP();
        console.log(WIFI_T, "Local IP address: " + IP.toString());
    }
    else
    {
        console.log(WIFI_T, "ERROR! Invalid Wi-Fi operating mode");
    }

    if(MDNS.begin("esp32"))
    {
        Serial.println("http://esp32.local/");
        MDNS.addService("http", "tcp", 80);
    }
}