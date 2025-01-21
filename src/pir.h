#pragma once
#include <Arduino.h>
#include <ESP32Time.h>

class PIR {
    private:
        int PIR_SENSOR;

    public:
        PIR(int pin);

        void init();
        void movementDetected();
};