#include "camera.h"
#include "camserver.h"
#include "pir.h"

const char* ssid = "HelloWorld";
const char* password = "12345677";

CameraServer camServer; //create a server for the camera on port 80
Camera camera;          //instantiate camera object 

PIR PIR(8); // placeholder number, please change to actual PIR pin number cos i don't have any hardware on me to test - max

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

    // call the movement detected somewhere here
}
