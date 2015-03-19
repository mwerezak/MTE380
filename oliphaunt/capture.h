#ifndef _CAPTURE_H
#define _CAPTURE_H

#include "action.h"
#include "utility.h"

/*
    Parameters:
    0 - floatval: The target angle
*/
class PanScoopServo : public SingletonAction<PanScoopServo> {
private:
    DelayTimer wait;
public:
    virtual char* getName() { return "PanScoopServo"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
};

/*
    Parameters:
    0 - floatval: The target angle
*/
class PanShovelServo : public SingletonAction<PanShovelServo> {
private:
    DelayTimer wait;
public:
    virtual char* getName() { return "PanShovelServo"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
};

#endif