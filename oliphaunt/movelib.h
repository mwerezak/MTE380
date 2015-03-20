#ifndef _MOVELIB_H
#define _MOVELIB_H

#include "action.h"
#include "utility.h"

#define FWD_FULL_SPEED  25.1 //cm/s

/*
    Parameters:
    0 - floatval: The target heading
*/
#define STOP_TOLERANCE 3.0  //degrees
#define SLOW_TOLERANCE 15.0
class TurnInPlaceToHeadingAction : public SingletonAction<TurnInPlaceToHeadingAction> {
private:
    float targetHeading;
    float targetBearing;
public:
    virtual char* getName() { return "TurnInPlaceToHeadingAction"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};



/*
    Parameters
    0 - floatval:   The x-position to drive to.
    1 - floatval:   The y-position to drive to.
    2 - floatval:   The navigation tolerance radius.
*/
class DriveToLocationAction : public SingletonAction<DriveToLocationAction> {
private:
    vector2 target_pos, current_pos;
    float target_bearing, angle_tolerance;
    float tolerance_rad;
public:
    virtual char* getName() { return "DriveToLocationAction"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};

/*
    Parameters:
    None
*/
class TestDriveAction : public SingletonAction<TestDriveAction> {
private:
    DelayTimer timer;
public:
    virtual char* getName() { return "TestDriveAction"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};


/*
    Parameters
    0 - floatval:   Distance to drive
*/
class DriveForwardsAction : public SingletonAction<DriveForwardsAction> {
private:
    DelayTimer timer;
public:
    virtual char* getName() { return "DriveForwardsAction"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};

/*
    Parameters
    0 - floatval:   The x-position to drive to.
    1 - floatval:   The y-position to drive to.
    2 - floatval:   The navigation tolerance radius.
*/
class DumbDriveToLocationAction : public SingletonAction<DumbDriveToLocationAction> {
private:
    vector2 target_pos;
    float tolerance_radius;
public:
    virtual char* getName() { return "DumbDriveToLocationAction"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};

/*
    Parameters
    None!
*/
class DriveUpRampAction : public SingletonAction<DriveUpRampAction> {
    DelayTimer done;
    boolean maybe_done;
public:
    virtual char* getName() { return "DriveUpRampAction"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};

#endif