
//#define DBG_GYRO_TRACKING
#define DBG_INS_TRACKING

#include "trackinglib.h"

#include <Arduino.h>
#include <math.h>
#include "numericlib.h"
#include "gyro_driver.h"
#include "accmag_driver.h"
#include "utility.h"

boolean gyro_enabled, acc_enabled;

AdamsBashforthIntegrator hdgIntegrator;
EulerIntegrator pitchIntegrator;

EulerIntegrator posXIntegrator;
EulerIntegrator posYIntegrator;

DelayTimer speedTimer;
AdamsBashforthIntegrator speedIntegrator;
AveragingFilter speedFilter;

void initTracking() {
    //initialize sensors
    Serial.println("Initializing gyro...");
    initGyro();
    
    Serial.println("Initializing accelerometer...");
    initAccMag();

    gyro_enabled = true;
    acc_enabled = false;
    
    //initialize integrators
    setCurrentHeading(0.0);
    setCurrentPitch(0.0);
    
    vector2 init_pos { .x = 0.0, .y = 0.0 };
    setCurrentPosition(init_pos);
}

void processTracking() {
    if(gyro_enabled && updateGyro()) {
        gyro_data gyro = getGyroReading();
        hdgIntegrator.feedData(gyro.GYRO_HDG_AXIS, gyro.update_time);
        pitchIntegrator.feedData(gyro.GYRO_PITCH_AXIS, gyro.update_time);
        
        #ifdef DBG_GYRO_TRACKING
        Serial.print("Hdg: ");
        Serial.print(getCurrentHeading());
        Serial.print(", Pitch: ");
        Serial.print(getCurrentPitch());
        Serial.println();
        #endif
    }
    
    if(acc_enabled && updateAcc()) {
        acc_data acc = getAccReading();
        float fwd_acc = acc.ACC_FWD_AXIS;
        
        if(fabs(fwd_acc) <= ACC_TOLERANCE) fwd_acc = 0.0;
        
        speedFilter.feedData(fwd_acc);
        speedIntegrator.feedData(speedFilter.getResult(), acc.update_time);
        
        if(doneSpeedMeasurement()) {
            acc_enabled = false;
            
            #ifdef DBG_INS_TRACKING
            Serial.print("Measured speed change: ");
            Serial.print(getMeasuredSpeed());
            Serial.println(" cm/s.");
            #endif
        }
    }
}

/** Gyro **/

void holdGyro() {
    gyro_enabled = false;
}

void releaseGyro() {
    float old_hdg = hdgIntegrator.getLastResult();
    float old_pitch = pitchIntegrator.getLastResult();
    hdgIntegrator.reset(old_hdg);
    pitchIntegrator.reset(old_pitch);
    gyro_enabled = true;
}

void setCurrentHeading(float newHdg) {
    hdgIntegrator.reset(newHdg);
}

float getCurrentHeading() {
    return normalizeAngle(hdgIntegrator.evalResult(millis()), 360);
}

void setCurrentPitch(float newPitch) {
    pitchIntegrator.reset(newPitch);
}

float getCurrentPitch() {
    return normalizeAngle(pitchIntegrator.evalResult(millis()), 180);
}


/** Dead-Reckoning **/

void setCurrentPosition(vector2 newPos) {
    posXIntegrator.reset(newPos.x);
    posYIntegrator.reset(newPos.y);
}

vector2 getCurrentPosition() {
    unsigned long cur_time = millis();
    vector2 pos;
    pos.x = posXIntegrator.evalResult(cur_time);
    pos.y = posYIntegrator.evalResult(cur_time);
    return pos;
}

void updateCurrentSpeed(float newspeed) {
    vector2 vel;
    
    //convert speed into X,Y components
    double theta = TORAD(getCurrentHeading());
    vel.x = newspeed*cos(theta);
    vel.y = newspeed*sin(theta);
    
    updateCurrentVelocity(vel);
}

void updateCurrentVelocity(vector2 new_vel) {
    unsigned long time = millis();
    posXIntegrator.feedData(new_vel.x, time);
    posYIntegrator.feedData(new_vel.y, time);
}

// Functions for using the accelerometer to measure changes in speed

void measureSpeedChange(unsigned long measure_time) {
    speedIntegrator.reset(0);
    speedFilter.reset();
    speedTimer.set(measure_time);
}

float getMeasuredSpeed() {
    return speedIntegrator.getLastResult();
}

boolean doneSpeedMeasurement() {
    return speedTimer.expired();
}

/** Compass **/



/** Helper Functions **/

//converts a heading into a bearing
float headingToBearing(float heading) {
    float bearing = normalizeAngle(heading - getCurrentHeading(), 180);
}

//updateHeading();

