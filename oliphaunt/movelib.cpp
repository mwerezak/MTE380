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
    return (fabs(targetBearing) <= STOP_TOLERANCE);
}

void TurnInPlaceToHeadingAction::doWork() {
    DriveCmd fwd = FULL_FWD;
    DriveCmd rev = FULL_REV;
    if(fabs(targetBearing) <= SLOW_TOLERANCE) {
        fwd = HALF_FWD;
        rev = HALF_REV;
    }
    
    if(targetBearing > 0) {
        driveServoLeft(fwd);
        driveServoRight(rev);
    }
    else {
        driveServoLeft(rev);
        driveServoRight(fwd);
    }
    
}

void TurnInPlaceToHeadingAction::cleanup() {
    driveServosNeutral();
}