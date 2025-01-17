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
    server.send(200, "text/html", index_html);
}

void CameraServer::serverInit(const char* ssid, const char* password){   //initialize server 
    wifiInit(ssid,password);
    server.on("/",[this](){ this->handleRoot();});
    server.on("/capture", HTTP_GET, [this](){ this->handleCapture(); });  //handle capture endpoint
    server.on("/list", HTTP_GET, [this](){ this->handleFileList(); });  //handle list endpoint
    server.on("/image", HTTP_GET, [this](){ this->handleImage(); });  //handle image endpoint
    server.begin();
    Serial.println("HTTP Server started!");
}

void CameraServer::handleCapture(){  // Handle "/capture" endpoint for taking a photo
    camera.capture();  // Call the capture method from the Camera class
    server.send(200, "text/plain", "Photo captured successfully");
}

void CameraServer::handleFileList(){
    String json = camera.createJSONFileList();
    server.send(200, "text/json", json);
}

void CameraServer::handleImage() {
  String imageFile = "/"+ server.arg("file");
  if (SD_MMC.exists(imageFile)) {
    File file = SD_MMC.open(imageFile);
    if (file) {
      server.streamFile(file, "image/jpeg");
      file.close();
    } else {
      server.send(500, "text/plain", "Failed to open image file");
    }
  } else {
    server.send(404, "text/plain", "File not found");
  }
}

void CameraServer::handleClient(){   //listent to cilent request 
    server.handleClient();
}

