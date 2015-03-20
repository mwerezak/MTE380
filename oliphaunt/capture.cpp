#include "capture.h"

#include "servolib.h"
#include "actionqueue.h"
#include "utility.h"

void startCapture() {
    ActionArgs angle_args, delay_args;
    ARGS(delay_args, 0, ulongval) = 1000;
    
    //ensure scoop is raised back (max angle)
    ARGS(angle_args, 0, floatval) = SERVO_SCOOP_MAX_ANGLE;
    queueAction(PanScoopServo::instance(), &angle_args);
    
    //open the shovel to -20 deg
    ARGS(angle_args, 0, floatval) = -20;
    queueAction(PanShovelServo::instance(), &angle_args);
    queueAction(WaitAction::instance(), &delay_args);
    
    //swing scoop in (min angle)
    ARGS(angle_args, 0, floatval) = SERVO_SCOOP_MIN_ANGLE;
    queueAction(PanScoopServo::instance(), &angle_args);
    queueAction(WaitAction::instance(), &delay_args);
    
    //retract scoop
    //ARGS(angle_args, 0, floatval) = SERVO_SCOOP_MAX_ANGLE;
    //ARGS(delay_args, 0, ulongval) = 500;
    //queueAction(PanScoopServo::instance(), &angle_args);
    //queueAction(WaitAction::instance(), &delay_args);
    
    //raise shovel (max angle)
    ARGS(angle_args, 0, floatval) = SERVO_SHOVEL_MAX_ANGLE;
    queueAction(PanShovelServo::instance(), &angle_args);
}

/** PanScoopServo **/

void PanScoopServo::setup(ActionArgs *args) {
    float target_angle = ARGSP(args, 0, floatval);
    
    wait.set(100);
    setScoopServo(target_angle);
}

boolean PanScoopServo::checkFinished() {
    return wait.expired();
}

/** PanIRServo **/

void PanShovelServo::setup(ActionArgs *args) {
    float target_angle = ARGSP(args, 0, floatval);
    
    wait.set(100);
    setShovelServo(target_angle);
}

boolean PanShovelServo::checkFinished() {
    return wait.expired();
}
