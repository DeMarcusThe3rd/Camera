#pragma once
#include <Arduino.h>
#include <ESP32Time.h>
#include "camera.h"

class PIR {
    private:
        int PIR_SENSOR;

    public:
        PIR(int pin);

        void init();
        void detectMovement(Camera *camera);
        void movementDetected(Camera *camera);
};