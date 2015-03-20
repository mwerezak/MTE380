#include "sensor_actions.h"

#include <Arduino.h>
#include "servolib.h"
#include "sensorlib.h"
#include "action.h"
#include "actionqueue.h"
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
    enableDistanceIR();
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

/** IRSweepForTarget **/

void IRSweepForTarget::setup(ActionArgs *args) {
    leftlimit   = ARGSP(args, 0, floatval);
    rightlimit  = ARGSP(args, 1, floatval);
    nearlimit   = ARGSP(args, 2, floatval);
    farlimit    = ARGSP(args, 3, floatval);
    float resolution = ARGSP(args, 4, floatval);
    
    return_ptr  = (float*)      ARGSP(args, 5, ptrval);
    found_ptr   = (boolean*)    ARGSP(args, 6, ptrval);
    
    angle_resolution = calcResolution(resolution);
    
    target_left = leftlimit;
    target_right = leftlimit;
    found_left = false;
    found_right = false;
    
    current_angle = leftlimit; //start at the leftmost limit
    scanAngle(current_angle);
}

float IRSweepForTarget::calcResolution(float arc_spacing) {
    float avg_rad = (farlimit + nearlimit)/2.0;
    float num_scan = avg_rad/arc_spacing;
    return fabs(rightlimit - leftlimit)/num_scan;
}

//pans the IR sensor to an angle and scans it, writing to last_reading
void IRSweepForTarget::scanAngle(float angle) {
    ActionArgs pan_args;
    ARGS(pan_args, 0, floatval) = angle;
    
    ActionArgs ir_args;
    ARGS(ir_args, 0, ptrval) = &last_reading;
    
    suspendCurrentAction();
    
    //pan to the angle, take a measurement, then resume
    setNextAction(this, NULL);
    setNextAction(IRDistanceReading::instance(), &ir_args);
    setNextAction(PanIRServo::instance(), &pan_args);
}

boolean IRSweepForTarget::checkFinished() {
    //found both corners
    if(found_left && found_right) {
        *found_ptr = true;
        *return_ptr = (target_right - target_left)/2.0;
        return true;
    }
    
    if(current_angle > rightlimit) {
        //found one corner but not the other
        if(found_left) {
            *found_ptr = true;
            *return_ptr = target_left;
        }
        else {
            *found_ptr = false;
        }
        return true;
    }
    
    return false;
}

void IRSweepForTarget::doWork() {
    //check if the reading is in the search range
    if(farlimit > last_reading && last_reading > nearlimit) {
        if(!found_left) {
            //found the left corner
            //found_left = true;
            target_left = current_angle;
        }
    }
    else {
        if(found_left && !found_right) {
            //found the right corner
            //found_right = true;
            target_right = current_angle;
        }
    }
    
    //scan the next angle
    current_angle += angle_resolution;
    scanAngle(current_angle);
}

void IRSweepForTarget::cleanup() {}

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