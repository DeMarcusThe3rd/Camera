#include "camera.h"
#include "camserver.h"
#include "pir.h"

const char* ssid = "Happyland";
const char* password = "afza5431";

CameraServer camServer; //create a server for the camera on port 80
Camera camera;          //instantiate camera object 

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
}
