#include "movelib.h"
#include "servolib.h"
#include "trackinglib.h"
#include <math.h>

/** TurnInPlaceToHeadingAction **/

void TurnInPlaceToHeadingAction::setup(ActionArgs *args) {
    targetHeading = ARGSP(args, 0, floatval);
    targetBearing = headingToBearing(targetHeading);
    
    driveServosStop();
}

boolean TurnInPlaceToHeadingAction::checkFinished() {
    targetBearing = headingToBearing(targetHeading);
    return (fabs(targetBearing) <= Tolerance);
}

void TurnInPlaceToHeadingAction::doWork() {
    char servoCommand = targetBearing/GainDiv;
    if(servoCommand < 10) servoCommand = 10;
    if(servoCommand > -10) servoCommand = -10;
    
    driveServoLeft(servoCommand);
    driveServoRight(-servoCommand);
}

void TurnInPlaceToHeadingAction::cleanup() {
    driveServosNeutral();
}