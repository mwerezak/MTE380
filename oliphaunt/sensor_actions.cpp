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
    ir_refresh.set(DISTIR_REFRESH_TIME);
}

boolean IRDistanceReading::checkFinished() {
    return (reading_count >= NumReadings);
}

void IRDistanceReading::doWork() {
    if(!ir_refresh.expired()) return;
    
    //take a reading
    readings[reading_count++] = (float) getDistanceIRReading();
    ir_refresh.reset();
}

void IRDistanceReading::cleanup() {
    //calculate the average and write it to return_ptr
    float total = 0;
    for(int i = 0; i < NumReadings; i++) {
        total += readings[i];
    }
    *return_ptr = total/NumReadings;
}

/** UltraSoundReading **/

void UltraSoundReading::setup(ActionArgs *args) {
    return_ptr = (float*) ARGSP(args, 0, ptrval);
    
    reading_count = 0;
    usound_refresh.set(USOUND_CYCLE_TIME+5);
}

boolean UltraSoundReading::checkFinished() {
    return (reading_count >= NumReadings);
}

void UltraSoundReading::doWork() {
    if(!usound_refresh.expired()) return;
    
    //take a reading
    usound_refresh.reset();
    readings[reading_count++] = (float) readUltraSound();
}

void UltraSoundReading::cleanup() {
    //calculate the average and write it to return_ptr
    boolean has_reading = false;
    float total = 0;
    for(int i = 0; i < NumReadings; i++) {
        if(readings[i] > 0) {
            total += readings[i];
            has_reading = true;
        }
    }
    if(has_reading) {
        *return_ptr = total/NumReadings;
    } else {
        *return_ptr = -1.0;
    }
}