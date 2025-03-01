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
    static unsigned long previousTime = 0; 

    if(camera->get_cooldown()==false){   //if cooldown is true, means still in cooldown mode, false means ready to take pic
        for(int i=0;i<camera->getpics_num();i++){
            digitalWrite(4, HIGH);
            camera->capture();

            previousTime = millis();
            while(millis() - previousTime <= 1500){  //wait for 1.5 seconds
            }
        }
        camera->set_cooldown(true);
        previousTime = millis(); //start cooldown timer
    }

    if(millis() - previousTime >= camera->getpics_interval()){  //if the cooldown is over, camera is ready to take pic again 
        camera->set_cooldown(false); 
    }
}