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
    server.on("/settings", HTTP_GET, [this](){ this->handleSettings(); });  //handle settings endpoint
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

void CameraServer::handleImage(){
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

void CameraServer::handleSettings(){
  if (server.hasArg("date") && server.hasArg("time")){
    String date = server.arg("date");
    String time = server.arg("time");

      int year, month, day, hour, minute;
      sscanf(date.c_str(), "%4d-%2d-%2d", &year, &month, &day);
      sscanf(time.c_str(), "%2d:%2d", &hour, &minute);

      camera.getDateTime().setTime(0, minute, hour, day, month, year);

      server.send(200, "text/plain", "Date and Time Updated Successfully!");
  }

  else{
    server.send(400, "text/plain", "Missing Date or Time!");
  }
}

void CameraServer::handleClient(){   //listen to cilent request 
    server.handleClient();
}

