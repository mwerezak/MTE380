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
    1 - ptrval (float*): A pointer to a float to write the reading.
*/
#define NUM_READINGS 5
class IRDistanceReading : public SingletonAction<IRDistanceReading> {
private:
    int reading_count;
    float readings[NUM_READINGS];
    DelayTimer ir_refresh;
    
    float *return_ptr;
public:
    virtual char* getName() { return "IRDistanceReading"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};

#endif
