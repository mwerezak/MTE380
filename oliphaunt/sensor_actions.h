#ifndef _SENSOR_ACTIONS_H
#define _SENSOR_ACTIONS_H

#include "action.h"
#include "utility.h"

/*
    Parameters:
    0 - floatval: The target angle
*/
class PanIRServo : public SingletonAction<PanIRServo> {
private:
    DelayTimer wait;
public:
    virtual char* getName() { return "PanIRServo"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
};

/*
    Parameters:
    0 - ptrval (float*): A pointer to a float to write the reading.
*/
class IRDistanceReading : public SingletonAction<IRDistanceReading> {
private:
    static const int NumReadings = 5;
    
    int reading_count;
    float readings[NumReadings];
    DelayTimer ir_refresh;
    
    float *return_ptr;
public:
    virtual char* getName() { return "IRDistanceReading"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};

/*
    Sweeps from left to right, and returns the midpoint of the first thing it finds.

    Parameters:
    0 - floatval - leftmost BEARING to sweep
    1 - floatval - rightmost BEARING to sweep
    2 - floatval - near limit
    3 - floatval - far limit
    4 - floatval - scan resolution, in cm
    5 - ptrval (float*) - pointer to a float to write the BEARING
    6 - ptrval (boolean*) - write true if something was found
*/
class IRSweepForTarget : public SingletonAction<IRSweepForTarget> {
    float leftlimit, rightlimit;
    float nearlimit, farlimit;
    float angle_resolution;
    float *return_ptr;
    boolean *found_ptr;
    
    boolean found_left, found_right;
    float current_angle;
    float last_reading;
    float target_left, target_right;
    
    float calcResolution(float arc_spacing);
    void scanAngle(float angle);
public:
    virtual char* getName() { return "IRSweepForTarget"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};



/*
    Parameters:
    0 - ptrval (float*): A pointer to a float to write the reading.
*/
class UltraSoundReading : public SingletonAction<UltraSoundReading> {
private:
    static const int NumReadings = 5;

    int reading_count;
    float readings[NumReadings];
    DelayTimer usound_refresh;
    
    float *return_ptr;
public:
    virtual char* getName() { return "UltraSoundReading"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};

#endif
