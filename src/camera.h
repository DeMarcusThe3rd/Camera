#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "soc/soc.h" // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include <ESP32Time.h>

class Camera
{
private: 
    // Camera pins
    #define PWDN_GPIO_NUM  32
    #define RESET_GPIO_NUM -1
    #define XCLK_GPIO_NUM  0
    #define SIOD_GPIO_NUM  26
    #define SIOC_GPIO_NUM  27

    #define Y9_GPIO_NUM    35
    #define Y8_GPIO_NUM    34
    #define Y7_GPIO_NUM    39
    #define Y6_GPIO_NUM    36
    #define Y5_GPIO_NUM    21
    #define Y4_GPIO_NUM    19
    #define Y3_GPIO_NUM    18
    #define Y2_GPIO_NUM    5
    #define VSYNC_GPIO_NUM 25
    #define HREF_GPIO_NUM  23
    #define PCLK_GPIO_NUM  22

    camera_config_t config;
    ESP32Time dateTime;
    int pics_num=1;
    unsigned long pics_interval=1;
    bool on_cooldown = true;

public: 
    void init();
    void sdInit();
    void createDIR(const char* path);  //creates a folder of that day 
    void clearCapture();
    void capture();  //take picture 
    String createJSONFileList();  //scans sd card for all files on it, and returns a json string
    void getImageFile(File *file, String filePath); 

    ESP32Time& getDateTime() {
        return dateTime;
    }

    void setpics_num(int pics_num){
        this->pics_num = pics_num;
    }

    int getpics_num(){
        return pics_num;
    }

    void setpics_interval(unsigned long pics_interval){
        this->pics_interval = pics_interval;
    }

    unsigned long getpics_interval(){
        return pics_interval;
    }

    bool get_cooldown(){
        return on_cooldown;
    }

    void set_cooldown(bool on_cooldown){
        this->on_cooldown = on_cooldown;
    }


};
