#ifndef _MOVELIB_H
#define _MOVELIB_H

#include "action.h"

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
*/
class DriveToLocationAction : public SingletonAction<DriveToLocationAction> {
public:
    virtual char* getName() { return "TurnInPlaceToHeadingAction"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};

#endif