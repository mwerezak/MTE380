#define DBG_MOVE_CONTROL

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
        rev = FULL_REV;
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
    
    #ifdef DBG_MOVE_CONTROL
    Serial.print("Pos: ");
    Serial.print(pos.x);
    Serial.print(", ");
    Serial.println(pos.y);
    #endif
}

void TestDriveAction::cleanup() {
    driveServosNeutral();
    updateCurrentSpeed(0); //notify tracking that we've stopped
    //holdGyro();
    
    //trap with the measured speed
    //Serial.println(getMeasuredSpeed());
}

/** DriveForwardsAction **/

//#include "sensorlib.h"

#define PROX_LIMIT 10.0

void DriveForwardsAction::setup(ActionArgs *args) {
    float distance = ARGSP(args, 0, floatval);
    timer.set(distance/FWD_FULL_SPEED*1000);
    
    setScoopServo(180); //so it doesn't mess with ultrasound
    
    driveServoLeft(FULL_FWD);
    driveServoRight(FULL_FWD);
}

boolean DriveForwardsAction::checkFinished() {
    //if(readUltraSound() <= PROX_LIMIT)
    //    return true;
    return timer.expired();
}

void DriveForwardsAction::doWork() {
    updateCurrentSpeed(FWD_FULL_SPEED);
}

void DriveForwardsAction::cleanup() {
    driveServosNeutral();
    updateCurrentSpeed(0);
}

/** DriveToLocationAction **/

void DriveToLocationAction::setup(ActionArgs *args) {
    target_pos.x = ARGSP(args, 0, floatval);
    target_pos.y = ARGSP(args, 1, floatval);
    tolerance_radius = ARGSP(args, 2, floatval);
}

boolean DriveToLocationAction::checkFinished() {

    //see if we're close enough
    vector2 current_pos = getCurrentPosition();
    float distance = getDistance(target_pos, current_pos);

    if(distance <= tolerance_radius) {
        return true; //done!
    }
    
    //okay, figure out how to get there
    ActionArgs turn_args, drive_args;
    ARGS(turn_args, 0, floatval) = getHeadingTo(target_pos);
    ARGS(drive_args, 0, floatval) = distance;

    #ifdef DBG_MOVE_CONTROL
    Serial.print("TGT: { ");
    Serial.print(target_pos.x);
    Serial.print(", ");
    Serial.print(target_pos.y);
    Serial.print(" }, ");
    Serial.print("DIST: ");
    Serial.print(distance);
    Serial.print(", HTT: ");
    Serial.print(ARGS(turn_args, 0, floatval));
    Serial.print(", BTT: ");
    Serial.println(headingToBearing(ARGS(turn_args, 0, floatval)));
    #endif
    
    //suspend ourselves, then put turn then drive at the front of the queue
    suspendCurrentAction();

    setNextAction(this, NULL);
    setNextAction(DriveForwardsAction::instance(), &drive_args);
    forceNextAction(TurnInPlaceToHeadingAction::instance(), &turn_args);
    
    return false;
}

void DriveToLocationAction::doWork() {}

void DriveToLocationAction::cleanup() {}

/** DriveUpRampAction **/

#define HORIZ_RAMP_LENGTH 220 //used to update our position afterwards
#define RAMP_TIMEOUT 250 //milliseconds
void DriveUpRampAction::setup(ActionArgs *args) {
    maybe_done = false;
    
    //we know we're level, so re-zero the gyro
    setCurrentPitch(0.0);
    
    driveServoLeft(FULL_FWD);
    driveServoRight(FULL_FWD);
    setScoopServo(40.0);
}

boolean DriveUpRampAction::checkFinished() {
    if(getCurrentPitch() < -20) {
        if(!maybe_done) {
            done.set(RAMP_TIMEOUT);
        }
        maybe_done = true;
    }
    else {
        maybe_done = false;
    }
    
    if(maybe_done && done.expired()) {
        return true;
    }
    
    return false;
}

void DriveUpRampAction::doWork() {}

void DriveUpRampAction::cleanup() {
    driveServosNeutral();
    
    //we're on the other side so adjust our position
    vector2 pos = getCurrentPosition();
    pos.y += HORIZ_RAMP_LENGTH;
    setCurrentPosition(pos);
}

