#define DBG_DRIVE_SERVOS

#include "servolib.h"

#include <Arduino.h>
#include <math.h>
#include "utility.h"

/** Drive Servos **/

void driveServosStop() {
    analogWrite(SERVO_DRIVE_LEFT_PIN, SERVO_DRIVE_LEFT_STOP);
    analogWrite(SERVO_DRIVE_RIGHT_PIN, SERVO_DRIVE_RIGHT_STOP);
}

void driveServosNeutral() {
    analogWrite(SERVO_DRIVE_LEFT_PIN, SERVO_DRIVE_LEFT_NEUTRAL);
    analogWrite(SERVO_DRIVE_RIGHT_PIN, SERVO_DRIVE_RIGHT_NEUTRAL);
}

void driveServoLeft(DriveCmd setting) {
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
    Serial.print("LEFT> setting: ");
    Serial.print(setting);
    Serial.print(", cmd: ");
    Serial.println(cmd);
    #endif
}

void driveServoRight(DriveCmd setting) {
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
    Serial.print("RIGHT> setting: ");
    Serial.print(setting);
    Serial.print(", cmd: ");
    Serial.println(cmd);
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