#include "capture.h"

#include "servolib.h"

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