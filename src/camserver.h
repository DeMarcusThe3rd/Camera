#pragma once
#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include "esp_camera.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "soc/soc.h" // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "index.h"
#include "camera.h"


class CameraServer
{
private:
    Camera *camera;
    WebServer server;  //esp webserver instance

public:
    CameraServer(Camera *cam,uint16_t port = 80) : camera(cam),server(port){};     //constructor  
    void wifiInit(const char* ssid, const char* password);  //initialize wifi 
    void handleRoot();      //handle on root (serves the first html page)
    void serverInit(const char* ssid, const char* password);      //initialize server 
    void handleClient();
    void handleCapture();
    void handleFileList();
    void handleImage();
    void handleSettings();
};
