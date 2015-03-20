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
    none!
*/
class SweepForBase : public SingletonAction<SweepForBase> {
private:
    float scan_angle, last_reading;
    int counter;
public:
    virtual char* getName() { return "SweepForBase"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void cleanup();
}


/*
    Parameters:
    0 - bool: is_lego_base? (true == lego, false == home)
    1 - bool: is_resume? (true == skip initial)
    2 - bool: is_straight (if base beside goes straight at it)
*/
class ReachBase : public SingletonAction<FindBase> {
private:
    int base_found;
    boolean is_lego_base;
    float d_2_base, a_2_base;
    const float TRACK_WIDTH = 280.0;
    const float TRACK_LENGHT_RAMP = 150.0;
    const float BASE_ACCU = 20.0;
    const float ADVANCE_INCR = 50.0;
public:
    virtual char* getName() { return "ReachBase"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
};

/*
    Parameters:
    0 - found_ret (boolean): Ptr to success or fail to write
    1 - dist_ptr (float *): Ptr to relative dist to write
    2 - angle_ptr (float *): Ptr to relative angle to write
*/
class FindTarget : public SingletonAction<FindBase> {
private:
    const float
public:
    virtual char* getName() { return "FindTarget"; }
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
