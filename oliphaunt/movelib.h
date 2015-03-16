#ifndef _MOVELIB_H
#define _MOVELIB_H

#include "action.h"

/*
    Parameters:
    0 - floatval: The target heading
*/
class TurnInPlaceToHeadingAction : public SingletonAction<TurnInPlaceToHeadingAction> {
private:
    static const int GainDiv = 10; 
    static const float Tolerance = 3; //degrees
    
    float targetHeading;
    float targetBearing;
public:
    virtual char* getName() { return "TurnInPlaceToHeadingAction"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};

#endif