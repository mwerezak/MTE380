/*
    World Coord System:
            0 deg
            Y
            ^
            |
    270 deg O - > X / 90 deg
    
    The +y axis points towards the side of the wall where the lego man is initially located.
    The +x axis points along the wall, and the origin is centered on it's left edge.
    Orientation in the world coord system are referred to as headings.
    
    Vehicle Coord System
            0 deg
            Y
            ^
            |
    -90 deg O - > X / +90 deg
    
    The +y axis points towards the front of the vehicle.
    The +x axis points towards the right side.
    Orientation in the vehicle coord system are referred to as bearings.
*/

#ifndef _TRACKINGLIB_H
#define _TRACKINGLIB_H

#define GYRO_HDG_AXIS x
#define GYRO_PITCH_AXIS z

#define ACC_Y_AXIS y
#define ACC_X_AXIS z

typedef struct Vector2Struct {
    float x;
    float y;
} vector2;


void setCurrentHeading(float newHdg);
void setCurrentPitch(float newPitch);
float getCurrentHeading();
float getCurrentPitch();

//Also sets the current velocity to zero
void setCurrentPosition(vector2 newPos);
vector2 getCurrentPosition();
vector2 getCurrentVelocity();

void initTracking();
void processTracking();

#endif