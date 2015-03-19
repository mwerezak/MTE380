
#define INERTIAL_TRACKING

//#define DBG_GYRO_TRACKING
#define DBG_INS_TRACKING

#include "trackinglib.h"

#include <Arduino.h>
#include <math.h>
#include "numericlib.h"
#include "gyro_driver.h"
#include "accmag_driver.h"
#include "utility.h"

AdamsBashforthIntegrator hdgIntegrator;
EulerIntegrator pitchIntegrator;

#ifdef INERTIAL_TRACKING
AdamsBashforthIntegrator velXIntegrator;
AdamsBashforthIntegrator velYIntegrator;
AdamsBashforthIntegrator posXIntegrator;
AdamsBashforthIntegrator posYIntegrator;
#else
EulerIntegrator posXIntegrator;
EulerIntegrator posYIntegrator;
#endif

void initTracking() {
    //initialize sensors
    Serial.println("Initializing gyro...");
    initGyro();
    
    Serial.println("Initializing accelerometer...");
    initAccMag();

    //initialize integrators
    setCurrentHeading(0.0);
    setCurrentPitch(0.0);
    
    vector2 init_pos { .x = 0.0, .y = 0.0 };
    setCurrentPosition(init_pos);
}

void processTracking() {
    if(updateGyro()) {
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
    
    
    #ifdef DBG_INS_TRACKING
    static unsigned long last_print = 0;
    #endif
    if(updateAcc()) {
        acc_data acc = getAccReading();
        
        velXIntegrator.feedData(acc.ACC_X_AXIS, acc.update_time);
        velYIntegrator.feedData(acc.ACC_Y_AXIS, acc.update_time);
        //posXIntegrator.feedData(velXIntegrator.getLastResult(), acc.update_time);
        //posYIntegrator.feedData(velYIntegrator.getLastResult(), acc.update_time);
        
        #ifdef DBG_INS_TRACKING
        if(millis() - last_print >= 100) {
            vector2 pos = getCurrentPosition();
            vector2 vel = getCurrentVelocity();
            Serial.print("Pos: { ");
            Serial.print(pos.x);
            Serial.print(", ");
            Serial.print(pos.y);
            Serial.print(" }, Vel: { ");
            Serial.print(vel.x);
            Serial.print(", ");
            Serial.print(vel.y);
            Serial.print(" }, Acc: { ");
            Serial.print(acc.ACC_X_AXIS);
            Serial.print(", ");
            Serial.print(acc.ACC_Y_AXIS);
            Serial.println(" };");
            last_print = millis();
        }
        #endif
    }
}

/** Gyro **/

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
    
    #ifdef INERTIAL_TRACKING
    velXIntegrator.reset(0.0);
    velYIntegrator.reset(0.0);
    #endif
}

vector2 getCurrentPosition() {
    #ifdef INERTIAL_TRACKING
    unsigned long cur_time = millis();
    vector2 pos;
    pos.x = posXIntegrator.evalResult(cur_time);
    pos.y = posYIntegrator.evalResult(cur_time);
    return pos;
    #else
    vector2 pos;
    return pos;
    #endif
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

vector2 getCurrentVelocity() {
    vector2 vel;
    #ifdef INERTIAL_TRACKING
    unsigned long cur_time = millis();
    vel.x = velXIntegrator.evalResult(cur_time);
    vel.y = velYIntegrator.evalResult(cur_time);
    #else
    //not supported
    vel.x = -1;
    vel.y = -1;
    #endif
    return vel;
}

/** Compass **/



/** Helper Functions **/

//converts a heading into a bearing
float headingToBearing(float heading) {
    float bearing = normalizeAngle(heading - getCurrentHeading(), 180);
}

//updateHeading();

