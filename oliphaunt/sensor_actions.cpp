#include "sensor_actions.h"

#include <Arduino.h>
#include "servolib.h"
#include "sensorlib.h"
#include "action.h"
#include "utility.h"

void PanIRAndTakeReading::setup(ActionArgs *args) {
    float target_angle = ARGSP(args, 0, floatval);
    result_ptr = (float*) ARGSP(args, 1, ptrval);
    
    reading_count = -1;
    wait_until = millis() + estimatePanningTime(target_angle) + 5;
    setPanningServo(target_angle);
}

boolean PanIRAndTakeReading::checkFinished() {
    if(reading_count == PanIRAndTakeReading::NumReadings) return true;
    
    return false;
}

void PanIRAndTakeReading::doWork() {
    if(millis() < wait_until) return; //waiting
    
    if(reading_count < 0) {
        reading_count = 0; //begin reading
        return;
    }
    
    if(reading_count < PanIRAndTakeReading::NumReadings) {
        readings[reading_count++] = (float) getDistanceIRReading();
        wait_until = millis() + 20; //wait 20 ms
        return;
    }
}

void PanIRAndTakeReading::cleanup() {
    float total = 0;
    for(int i = 0; i < PanIRAndTakeReading::NumReadings; i++) {
        total += readings[i];
    }
    *result_ptr = total/PanIRAndTakeReading::NumReadings;
}
