#include <Arduino.h>
#include <ESP32Time.h>
#include <FunctionalInterrupt.h>
#include "pir.h"
#include "camera.h"

PIR::PIR(int pin){
    PIR_SENSOR = pin;
}

void PIR::init(){
    pinMode(PIR_SENSOR, INPUT_PULLDOWN);
    pinMode(4, OUTPUT);
}

void PIR::detectMovement(Camera *camera){
    if (digitalRead(PIR_SENSOR) == HIGH){
        movementDetected(camera);
    }
    else{
        digitalWrite(4, LOW);
        // possibility of exploring low power mode on the esp32 when no movement is detected?
    }
}

void PIR::movementDetected(Camera *camera){
    digitalWrite(4, HIGH);
    unsigned long previousTime = millis(); //start timing 

    for(int i=0;i<camera->getpics_num();i++){
        unsigned long currentTime = millis();
        while(currentTime - previousTime < camera->getpics_interval()){
            currentTime = millis();  //update time until after interval
        }
        camera->capture();
        previousTime = currentTime;
    }

}