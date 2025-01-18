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
    server.on("/stream", HTTP_GET, [this](){ this->handleStream(); });  //handle settings endpoint
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

void CameraServer::handleStream(){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "multipart/x-mixed-replace; boundary=123456789000000000000987654321\r\n");

  while (true) {
    // Check if the client is still connected
    if (!server.client().connected()) {
        break;
    }

    camera_fb_t* frame = camera.captureFrame();
    if (!frame) {
        Serial.println("Failed to capture frame");
        break;
    }

    String header = "--123456789000000000000987654321\r\n"
                    "Content-Type: image/jpeg\r\n"
                    "Content-Length: " + String(frame->len) + "\r\n\r\n";
    server.client().write(header.c_str(), header.length());
    server.client().write(frame->buf, frame->len);
    server.client().write("\r\n");

    // Release the frame buffer
    camera.releaseFrame(frame);

  }
}

void CameraServer::handleClient(){   //listen to cilent request 
    server.handleClient();
}

