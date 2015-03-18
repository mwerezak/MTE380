#include "sensor_actions.h"

#include <Arduino.h>
#include "servolib.h"
#include "sensorlib.h"
#include "action.h"
#include "utility.h"

/** PanIRServo **/

void PanIRServo::setup(ActionArgs *args) {
    float target_angle = ARGSP(args, 0, floatval);
    
    wait.set(estimatePanningTime(target_angle) + 5);
    setPanningServo(target_angle);
}

boolean PanIRServo::checkFinished() {
    return wait.expired();
}


/** IRDistanceReading **/

void IRDistanceReading::setup(ActionArgs *args) {
    return_ptr = (float*) ARGSP(args, 0, ptrval);
    
    reading_count = 0;
    this->ir_refresh.set(DISTIR_REFRESH_TIME);
}

boolean IRDistanceReading::checkFinished() {
    return (reading_count >= NUM_READINGS);
}

void IRDistanceReading::doWork() {
    if(!this->ir_refresh.expired()) return;
    
    //take a reading
    readings[reading_count++] = (float) getDistanceIRReading();
    this->ir_refresh.reset();
}

void IRDistanceReading::cleanup() {
    //calculate the average and write it to return_ptr
    float total = 0;
    for(int i = 0; i < NUM_READINGS; i++) {
        total += readings[i];
    }
    *return_ptr = total/NUM_READINGS;
}