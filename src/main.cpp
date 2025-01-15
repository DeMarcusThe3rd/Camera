#include "camera.h"
#include "camserver.h"

const char* ssid = "HelloWorld";
const char* password = "12345677";

CameraServer camServer; //create a server for the camera on port 80
Camera camera;          //instantiate camera object 

void setup() {
    Serial.begin(115200);
    camera.init();
    camera.sdInit();
    camServer.serverInit(ssid,password);

}

void loop() {
    camServer.handleClient();
}
