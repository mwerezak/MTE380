#include "utility.h"
#include "trackinglib.h"

#include <Arduino.h>
#include <math.h>

#ifdef DBG_PRINT
char __printbuf[PRINT_BUF_SIZE];
#endif

#ifndef PI
    #define PI 3.141592
#endif

template <typename T>
void printArray(T *array, int len, char *name) {
    Serial.print(name);
    Serial.print("[]: { ");
    for(int i = 0; i < len-1; i++) {
        Serial.print(array[i]);
        Serial.print(", ");
    }
    Serial.print(array[len-1]);
    Serial.println(" };");
}

/** Geometry/Nav Helpers **/

float normalizeAngle(float angle, float max_angle) {
    float min_angle = max_angle - 360;
    while (angle <= min_angle) { angle += 360; }
    while (angle >  max_angle) { angle -= 360; }
    return angle;
}

float getShortestArc(float angle1, float angle2) {
    float arc = normalizeAngle(angle1 - angle2, 360);
    return (arc > 180.0)? 360.0 - arc : arc;
}

float getDistance(vector2 p1, vector2 p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return hypot(dx, dy);
}

vector2 getAbsoluteDisplacement(float distance, float angle) {
    vector2 current_location = getCurrentPosition();
    float angle_abs = angle + getCurrentHeading();
    float x2 = distance*sin(TORAD(angle_abs));
    float y2 = distance*cos(TORAD(angle_abs));
    vector2 retVal = {current_location.x + x2, current_location.y + y2};
    return retVal;
}

/** Delay Timer **/

void DelayTimer::set(unsigned long _delay_time) {
    delay_time = _delay_time;
    reset();
}

void DelayTimer::reset() {
    last_set_time = millis();
}

boolean DelayTimer::expired() {
    return (millis() >= delay_time + last_set_time);
}

/** WaitAction **/

void WaitAction::setup(ActionArgs *args) {
    unsigned long wait_time = ARGSP(args, 0, ulongval);
    wait.set(wait_time);
}
boolean WaitAction::checkFinished() {
    return wait.expired();
}