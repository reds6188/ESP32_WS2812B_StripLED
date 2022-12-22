#include <Arduino.h>
#include <WiFi.h>
#include "wi-fi.h"
#include <ESPmDNS.h>

// Replace with your network credentials
/*
const char* ssid     = "FanHeaterV1";
const char* password = "123456789";
*/
/*
const char* ssid     = "Martin Router King";
const char* password = "4ge0f3mpires-Rise0fRome";
*/
/*
const char* ssid     = "R0X";
const char* password = "4ge0f3mpires";
*/
/*
const char* ssid     = "Eltek-9F4040";
const char* password = "673541008998";
*/
/*
const char* ssid     = "FAIRWIND_WIFI";
const char* password = "Fair2020";
*/

const char* ssid     = "HAFRO§_Vasca_Bolla";
const char* password = "123456789";

long start_wifi_millis;

bool checkConnection(void)
{
    start_wifi_millis = millis();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        if(millis() - start_wifi_millis > WIFI_TIMEOUT)
        {
            WiFi.disconnect(true, true);
            return false;
        }
    }
    return true;
}

void initWiFi(uint8_t mode)
{
    Serial.println(">> Starting Wi-Fi configuration...");
    if(mode == AP_MODE)
    {
        IPAddress localIP(192, 168, 4, 1);    // Set your Static IP address
        IPAddress gatewayAddr(192, 168, 4, 0);  // Set your Gateway IP address
        IPAddress subnetMask(255, 255, 0, 0);   // Set your Subnet Mask

        Serial.println(">> Setting AP (Access Point)...");
        WiFi.mode(WIFI_AP);

        if(!WiFi.softAP(ssid, password))
        {
            Serial.println(">> AP failed to start");
        }
        delay(100);
        if(!WiFi.softAPConfig(localIP, gatewayAddr, subnetMask))
        {
            Serial.println(">> AP configuration failed");
        }
        else
        {
            Serial.println(">> AP started!");
            Serial.print(">> Network SSID: ");
            Serial.println(String(ssid));
            IPAddress IP = WiFi.softAPIP();
            Serial.print(">> AP IP address: ");
            Serial.println(IP);
        }
    }
    else if(mode == STA_MODE)
    {
        Serial.println(">> Setting STA (STAtion mode)...");
        WiFi.mode(WIFI_STA);
        while(!checkConnection())
        {
            Serial.println();
            Serial.println(">> Wi-Fi error: failed to connect.");
            Serial.println(">> Connection restart...");
        }
        Serial.println();
        Serial.println(">> Wi-Fi connected!");
        Serial.print(">> Network SSID: ");
        Serial.println(String(ssid));
        Serial.print(">> Local IP address: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println(">> ERROR! Invalid Wi-Fi operating mode");
    }

    if(MDNS.begin("esp32"))
    {
        Serial.println("http://esp32.local/");
        MDNS.addService("http", "tcp", 80);
    }
}