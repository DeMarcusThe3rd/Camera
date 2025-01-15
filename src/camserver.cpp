#include "camserver.h"

void CameraServer::wifiInit(const char* ssid, const char* password){  //connects to wifi 
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }
    Serial.println("Connected to Wi-Fi!");
    Serial.print("IP Address: ");   
    Serial.println(WiFi.localIP()); //ip address for esp32 (webserver)
}

void CameraServer::handleRoot(){   //handle "/" (root endpoint) case  
    server.send(200, "text/html", HTML_PAGE);
}

void CameraServer::serverInit(const char* ssid, const char* password){   //initialize server 
    wifiInit(ssid,password);
    server.on("/",[this](){ this->handleRoot();});
    server.on("/stream",[this](){ this->handleStream();});
    server.begin();
    Serial.println("HTTP Server started!");
}

void CameraServer::handleClient(){   //listent to cilent request 
    server.handleClient();
}

void CameraServer::handleStream() {  
    camera_fb_t* fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t* _jpg_buf = NULL;
    char part_buf[64];  // Fix to char array, to ensure proper usage

    server.sendHeader("Content-Type", "multipart/x-mixed-replace;boundary=123456789000000000000987654321");
    if (res != ESP_OK) {
        Serial.println("Error initializing stream.");
        return;
    }

    while (true) {
        fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            res = ESP_FAIL;
            break;
        } else {
            if (fb->width > 400) {
                if (fb->format != PIXFORMAT_JPEG) {
                    bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
                    esp_camera_fb_return(fb);
                    fb = NULL;
                    if (!jpeg_converted) {
                        Serial.println("JPEG compression failed");
                        res = ESP_FAIL;
                        break;
                    }
                } else {
                    _jpg_buf_len = fb->len;
                    _jpg_buf = fb->buf;
                }
            }
        }
        
        if (res == ESP_OK) {
            size_t hlen = snprintf(part_buf, sizeof(part_buf), "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", _jpg_buf_len);
            server.sendContent(part_buf, hlen);
        }

        if (res == ESP_OK) {
            server.sendContent((const char*)_jpg_buf, _jpg_buf_len);
        }

        if (res == ESP_OK) {
            server.sendContent("\r\n--123456789000000000000987654321\r\n", 44);
        }

        if (fb) {
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        } else if (_jpg_buf) {
            free(_jpg_buf);
            _jpg_buf = NULL;
        }
        
        if (res != ESP_OK) {
            break;  // exit the loop if an error occurs
        }
    }
}

