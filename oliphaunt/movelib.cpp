#include "movelib.h"

#include <math.h>
#include "servolib.h"
#include "trackinglib.h"
#include "actionqueue.h"
#include "utility.h"

/** TurnInPlaceToHeadingAction **/

void TurnInPlaceToHeadingAction::setup(ActionArgs *args) {
    targetHeading = ARGSP(args, 0, floatval);
    targetBearing = headingToBearing(targetHeading);
    
    driveServosStop();
    //releaseGyro();
}

boolean TurnInPlaceToHeadingAction::checkFinished() {
    targetBearing = headingToBearing(targetHeading);
    return (fabs(targetBearing) <= STOP_TOLERANCE);
}

void TurnInPlaceToHeadingAction::doWork() {
    DriveCmd fwd = FULL_FWD;
    DriveCmd rev = FULL_REV;
    if(fabs(targetBearing) <= SLOW_TOLERANCE) {
        fwd = FULL_FWD;
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
    //holdGyro();
}

/** DriveToLocationAction **/

void DriveToLocationAction::setup(ActionArgs *args) {

    target_pos.x = ARGSP(args, 0, floatval);
    target_pos.y = ARGSP(args, 1, floatval);
    tolerance_rad = ARGSP(args, 2, floatval);
    
    //releaseGyro();
    updateCurrentSpeed(25.1);
}

boolean DriveToLocationAction::checkFinished() {;

    current_pos = getCurrentPosition();
    float target_heading = getHeadingTo(target_pos);
    target_bearing = headingToBearing(target_heading);
    float distance = getDistance(target_pos, current_pos);
    angle_tolerance = fabs(atan2(tolerance_rad, distance));
    
    Serial.print("Pos: { ");
    Serial.print(current_pos.x);
    Serial.print(", ");
    Serial.print(current_pos.y);
    Serial.print(" }, Tgt: { ");
    Serial.print(target_pos.x);
    Serial.print(", ");
    Serial.print(target_pos.y);
    Serial.print(" }, BTT: ");
    Serial.print(target_bearing);
    Serial.print(" }, HTT: ");
    Serial.print(target_heading);
    Serial.print(", DST: ");
    Serial.print(distance);
    Serial.print(", EPS: ");
    Serial.println(angle_tolerance);
    
    //Check if we've reached the destination
    if(distance <= tolerance_rad/3.0)
        return true;
    
    //Check if we've overshot, but are close enough that it's not worth turning
    //around to get a little bit closer.
    if(distance <= tolerance_rad && fabs(target_bearing) > angle_tolerance)
        return true;
    
    return false; //keep going
}

void DriveToLocationAction::doWork() {
    //Check if we've gone too far off course. If so, stop and turn.
    if(fabs(target_bearing) > angle_tolerance) {
        ActionArgs turn_args, drive_args;
        ARGS(turn_args, 0, floatval) = getHeadingTo(target_pos);
        
        ARGS(drive_args, 0, floatval) = target_pos.x;
        ARGS(drive_args, 1, floatval) = target_pos.y;
        ARGS(drive_args, 2, floatval) = tolerance_rad;
        
        updateCurrentSpeed(0);
        forceNextAction(TurnInPlaceToHeadingAction::instance(), &turn_args); //kills the current action
        setNextAction(this, &drive_args);
    } else {
        driveServoLeft(FULL_FWD);
        driveServoRight(FULL_FWD);
        updateCurrentSpeed(FWD_FULL_SPEED);
    }
}

void DriveToLocationAction::cleanup() {
    driveServosNeutral();
    updateCurrentSpeed(0); //notify tracking that we've stopped
}

/** TestDriveAction **/

void TestDriveAction::setup(ActionArgs *args) {
    timer.set(5000);
    
    //releaseGyro();
    driveServoLeft(FULL_FWD);
    driveServoRight(FULL_FWD);
}

boolean TestDriveAction::checkFinished() {
    return timer.expired();
}

void TestDriveAction::doWork() {
    updateCurrentSpeed(FWD_FULL_SPEED);
    vector2 pos = getCurrentPosition();
    
    Serial.print("Pos: ");
    Serial.print(pos.x);
    Serial.print(", ");
    Serial.println(pos.y);
}

void TestDriveAction::cleanup() {
    driveServosNeutral();
    updateCurrentSpeed(0); //notify tracking that we've stopped
    //holdGyro();
    
    //trap with the measured speed
    //Serial.println(getMeasuredSpeed());
}


