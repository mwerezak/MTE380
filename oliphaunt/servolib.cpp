//#define DBG_DRIVE_SERVOS

#include "servolib.h"

#include <Arduino.h>
#include <math.h>
#include "utility.h"

/** Drive Servos **/

static unsigned long last_update_left, last_update_right;
static boolean update_left, update_right;
static DriveCmd command_left, command_right;

void _setDriveServoLeft(DriveCmd setting);
void _setDriveServoRight(DriveCmd setting);

void initDriveServos() {
    unsigned long time = millis();
    last_update_left = time;
    last_update_right = time;

    update_left = false;
    update_right = false;
}

void processDriveServos() {
    if(update_left && millis() >= last_update_left + SERVO_DRIVE_DEADTIME) {
        _setDriveServoLeft(command_left);
        last_update_left = millis();
        update_left = false;
    }
    if(update_right && millis() >= last_update_right + SERVO_DRIVE_DEADTIME) {
        _setDriveServoRight(command_right);
        last_update_right = millis();
        update_right = false;
    }
}

void driveServosStop() {
    driveServoLeft(HOLD_STOP);
    driveServoRight(HOLD_STOP);
}

void driveServosNeutral() {
    driveServoLeft(NEUTRAL);
    driveServoRight(NEUTRAL);
}

void driveServoLeft(DriveCmd command) {
    command_left = command;
    update_left = true;
}

void driveServoRight(DriveCmd command) {
    command_right = command;
    update_right = true;
}

void _setDriveServoLeft(DriveCmd setting) {
    byte cmd = 0;
    switch(setting) {
        case FULL_FWD:
            cmd = SERVO_DRIVE_LEFT_FULL_FWD;
            break;
        case HALF_FWD:
            cmd = SERVO_DRIVE_LEFT_HALF_FWD;
            break;
        case FULL_REV:
            cmd = SERVO_DRIVE_LEFT_FULL_REV;
            break;
        case HALF_REV:
            cmd = SERVO_DRIVE_LEFT_HALF_REV;
            break;
        case HOLD_STOP:
            cmd = SERVO_DRIVE_LEFT_STOP;
            break;
        case NEUTRAL:
            cmd = SERVO_DRIVE_LEFT_NEUTRAL;
            break;
    }
    analogWrite(SERVO_DRIVE_LEFT_PIN, cmd);
    
    #ifdef DBG_DRIVE_SERVOS
    PRINTF("SERVO LEFT> %d", setting);
    #endif
}

void _setDriveServoRight(DriveCmd setting) {
    byte cmd = 0;
    switch(setting) {
        case FULL_FWD:
            cmd = SERVO_DRIVE_RIGHT_FULL_FWD;
            break;
        case HALF_FWD:
            cmd = SERVO_DRIVE_RIGHT_HALF_FWD;
            break;
        case FULL_REV:
            cmd = SERVO_DRIVE_RIGHT_FULL_REV;
            break;
        case HALF_REV:
            cmd = SERVO_DRIVE_RIGHT_HALF_REV;
            break;
        case HOLD_STOP:
            cmd = SERVO_DRIVE_RIGHT_STOP;
            break;
        case NEUTRAL:
            cmd = SERVO_DRIVE_RIGHT_NEUTRAL;
            break;
    }
    analogWrite(SERVO_DRIVE_RIGHT_PIN, cmd);
    
    #ifdef DBG_DRIVE_SERVOS
    PRINTF("SERVO RIGHT> %d", setting);
    #endif
}


/** Panning Servo **/
static float panning_setpoint;

void setPanningServo(float angle) {
    float fcmd = LINSCALE(angle, -90, +90, SERVO_PANNING_MIN, SERVO_PANNING_MAX);
    byte cmd = (byte) round(fcmd);
    analogWrite(SERVO_PANNING_PIN, cmd);
    panning_setpoint = angle;
}

float getPanningServoSetpoint() {
    return panning_setpoint;
}

unsigned long estimatePanningTime(float target_angle) {
    float arc = getShortestArc(panning_setpoint, target_angle);
    return (unsigned long) ceil(arc*SERVO_PANNING_EST_SPEED);
}