#include <Arduino.h>
#include <ESP32Time.h>
#include "pir.h"

PIR::PIR(int pin){
    PIR_SENSOR = pin;
}

void PIR::init(){
    pinMode(PIR_SENSOR, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIR_SENSOR), movementDetected, RISING);
}

void IRAM_ATTR PIR::movementDetected(){
    // Code to activate camera when movement is detected
    // possibility of exploring low power mode on the esp32 when no movement is detected?
}