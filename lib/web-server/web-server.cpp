#include "web-server.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void wsUpdateMsg(String msg)
{
    ws.textAll(msg);
}

void initWebSocket(AwsEventHandler event) {
    ws.onEvent(event);
    server.addHandler(&ws);
}

void wsCleanupClients(void) {
    ws.cleanupClients();
}

void initWebServer(AwsTemplateProcessor callback)
{
    if(!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    //server.on("^(\\/.+\\/)*(.+)\.(.+)$", HTTP_GET, [](AsyncWebServerRequest *request){
    server.on("^(\\/.+\\/)*(.+)\\.(.+)$", HTTP_GET, [](AsyncWebServerRequest *request){
        String dir = request->pathArg(0);
        String name = request->pathArg(1);
        String ext = request->pathArg(2);
        Serial.println("dir = " + dir);
        Serial.println("name = " + name);
        Serial.println("ext = " + ext);
        String MIMEtype = "text/plain";
        String path = dir + name + "." + ext;
        if(!ext.compareTo("svg")) {
            Serial.println("Serving SVG file");
            MIMEtype = "image/svg+xml";
        }
        else if(!ext.compareTo("css")) {
            Serial.println("Serving CSS file");
            MIMEtype = "text/css";
        }
        else if(!ext.compareTo("js")) {
            Serial.println("Serving JS file");
            MIMEtype = "text/javascript";
        }
        else if(!ext.compareTo("ico")) {
            Serial.println("Serving ICO file");
            MIMEtype = "image/x-icon";
        }
        else
            Serial.println("ERROR: path not recognized");
        request->send(SPIFFS, path, MIMEtype);
    });

    // Route for root / web page
    server.on("/", HTTP_GET, [callback](AsyncWebServerRequest *request){
        Serial.println("Serving index.html");
        //request->send(SPIFFS, "/index.html", "text/html");
        request->send(SPIFFS, "/index.html", String(), false, callback);
    });

    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "ERROR 404 - The content you are looking for was not found.");
    });

    // Start server
    server.begin();
}