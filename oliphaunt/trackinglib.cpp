#include "trackinglib.h"

unsigned long _hdgLastUpdateTime, _posLastUpdateTime;

float normalizeAngle(float angle, float max_angle=360) {
    float min_angle = max_angle - 360;
    while (angle <= min_angle) { angle += 360; }
    while (angle >  max_angle) { angle -= 360; }
    return angle;
}

//converts a heading into a bearing
float headingToBearing(float heading) {
    float bearing = normalizeAngle(heading - getCurrentHeading(), 180);
}

float getCurrentHeading() {
    return 0;
}

//updateHeading();

