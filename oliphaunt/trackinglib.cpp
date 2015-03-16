
#include "trackinglib.h"

#include <Arduino.h>
#include "numericlib.h"
#include "gyro_driver.h"

EulerIntegrator hdgIntegrator;
EulerIntegrator pitchIntegrator;

void initTracking() {
    //initialize sensors
    Serial.println("Initializing gyro...");
    initGyro();

    //initialize integrators
    setCurrentHeading(0.0);
    setCurrentPitch(0.0);
}

void processTracking() {
    if(updateGyro()) {
        gyro_data gyro = getGyroReading();
        hdgIntegrator.feedData(gyro.x, gyro.update_time);
        pitchIntegrator.feedData(gyro.z, gyro.update_time);
        
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
    return normalizeAngle(hdgIntegrator.getResult(millis()), 360);
}

void setCurrentPitch(float newPitch) {
    pitchIntegrator.reset(newPitch);
}

float getCurrentPitch() {
    return normalizeAngle(pitchIntegrator.getResult(millis()), 180);
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

