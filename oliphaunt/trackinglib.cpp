
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
AdamsBashforthIntegrator pitchIntegrator;

EulerIntegrator posXIntegrator;
EulerIntegrator posYIntegrator;

void initTracking() {
    //initialize sensors
    Serial.println("Initializing gyro...");
    initGyro();
    
    //Serial.println("Initializing accelerometer...");
    //initAccMag();

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
}

vector2 getCurrentPosition() {
    unsigned long time = millis();

    vector2 pos;
    pos.x = posXIntegrator.evalResult(time);
    pos.y = posYIntegrator.evalResult(time);
    return pos;
}

void updateCurrentSpeed(float newspeed) {
    vector2 vel;
    
    //convert speed into X,Y components
    double theta = TORAD(getCurrentHeading());
    vel.x = newspeed*sin(theta);
    vel.y = newspeed*cos(theta);
    
    #ifdef DBG_INS_TRACKING
    Serial.print("speed> ");
    Serial.println(newspeed);
    #endif
    
    updateCurrentVelocity(vel);
}

void updateCurrentVelocity(vector2 new_vel) {
    unsigned long time = millis();
    posXIntegrator.feedData(new_vel.x, time);
    posYIntegrator.feedData(new_vel.y, time);
    
    #ifdef DBG_INS_TRACKING
    Serial.print("Pos: { ");
    Serial.print(posXIntegrator.getLastResult());
    Serial.print(", ");
    Serial.print(posYIntegrator.getLastResult());
    Serial.println(" }");
    #endif
}

/** Helper Functions **/

//converts a heading into a bearing
float headingToBearing(float heading) {
    float bearing = normalizeAngle(heading - getCurrentHeading(), 180);
}

float getHeadingTo(vector2 loc) {
    vector2 pos = getCurrentPosition();
    
    //put ourselves at the origin
    loc.x -= pos.x;
    loc.y -= pos.y;
    
    float angle = atan2(loc.x, loc.y);
    return normalizeAngle(TODEG(angle), 360);
}

