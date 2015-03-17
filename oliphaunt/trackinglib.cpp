
//#define DBG_GYRO_TRACKING
#define DBG_INS_TRACKING

#include "trackinglib.h"

#include <Arduino.h>
#include "numericlib.h"
#include "gyro_driver.h"
#include "accmag_driver.h"

AdamsBashforthIntegrator hdgIntegrator;
EulerIntegrator pitchIntegrator;

AdamsBashforthIntegrator velXIntegrator;
AdamsBashforthIntegrator velYIntegrator;

AdamsBashforthIntegrator posXIntegrator;
AdamsBashforthIntegrator posYIntegrator;

#ifdef DBG_INS_TRACKING
unsigned long last_print;
#endif

void initTracking() {
    //initialize sensors
    Serial.println("Initializing gyro...");
    initGyro();
    
    Serial.println("Initializing accelerometer/magnetometer...");
    initAccMag();

    //initialize integrators
    setCurrentHeading(0.0);
    setCurrentPitch(0.0);
    
    #ifdef DBG_INS_TRACKING
    last_print = millis();
    #endif
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
    
    if(updateAcc()) {
        acc_data acc = getAccReading();
        
        velXIntegrator.feedData(acc.ACC_X_AXIS, acc.update_time);
        velYIntegrator.feedData(acc.ACC_Y_AXIS, acc.update_time);
        
        posXIntegrator.feedData(velXIntegrator.getLastResult(), acc.update_time);
        posYIntegrator.feedData(velYIntegrator.getLastResult(), acc.update_time);
        
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
            Serial.print(acc.x);
            Serial.print(", ");
            Serial.print(acc.y);
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


/** Accelerometer **/

void setCurrentPosition(vector2 newPos) {
    posXIntegrator.reset(newPos.x);
    posYIntegrator.reset(newPos.y);
    
    velXIntegrator.reset(0);
    velYIntegrator.reset(0);
}

vector2 getCurrentPosition() {
    unsigned long cur_time = millis();
    vector2 pos;
    pos.x = posXIntegrator.evalResult(cur_time);
    pos.y = posYIntegrator.evalResult(cur_time);
    return pos;
}

vector2 getCurrentVelocity() {
    unsigned long cur_time = millis();
    vector2 vel;
    vel.x = velXIntegrator.evalResult(cur_time);
    vel.y = velYIntegrator.evalResult(cur_time);
    return vel;
}

/** Compass **/







/** Helper Functions **/

float normalizeAngle(float angle, float max_angle) {
    float min_angle = max_angle - 360;
    while (angle <= min_angle) { angle += 360; }
    while (angle >  max_angle) { angle -= 360; }
    return angle;
}

//converts a heading into a bearing
float headingToBearing(float heading) {
    float bearing = normalizeAngle(heading - getCurrentHeading(), 180);
}

//updateHeading();

