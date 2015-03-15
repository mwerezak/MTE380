#ifndef _TRACKINGLIB_H
#define _TRACKINGLIB_H

float normalizeAngle(float angle);
float headingToBearing(float heading);

float getCurrentHeading();
void updatePosition();
void updateHeading();

//updateHeading();

#endif