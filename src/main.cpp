#include "camera.h"
#include "camserver.h"
#include "pir.h"

const char* ssid = "ESP32-CAM Access Point";
const char* password = "123456789";

Camera camera;          //instantiate camera object 
CameraServer camServer(&camera); //create a server for the camera on port 80

PIR pir(13); // PIR sensor pin 13 on prototype board

void setup() {
    Serial.begin(115200);
    camera.init();
    camera.sdInit();
    camServer.serverInit(ssid,password);

    // PIR Sensor
    pir.init();
}

void loop() {
    camServer.handleClient();
    pir.detectMovement(&camera);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}
