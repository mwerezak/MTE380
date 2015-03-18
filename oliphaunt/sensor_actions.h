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
    Parameters:
    1 - ptrval (float*): A pointer to a float to write the reading.
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
