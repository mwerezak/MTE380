#include "servolib.h"

#include <Arduino.h>
#include <math.h>
#include "utility.h"

/** Drive Servos **/

void driveServosStop() {
    analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, SERVO_DRIVE_LEFT_PWM_STOP);
    analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, SERVO_DRIVE_RIGHT_PWM_STOP);
}

void driveServosNeutral() {
    analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, SERVO_DRIVE_LEFT_PWM_NEUTRAL);
    analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, SERVO_DRIVE_RIGHT_PWM_NEUTRAL);
}

void driveServoLeftCmd(byte command) {
    byte pwm = constrain(command, SERVO_DRIVE_LEFT_PWM_MIN, SERVO_DRIVE_LEFT_PWM_MAX);
    analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, pwm);
}

void driveServoRightCmd(byte command) {
    byte pwm = constrain(command, SERVO_DRIVE_RIGHT_PWM_MIN, SERVO_DRIVE_RIGHT_PWM_MAX);
    analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, pwm);
}

void driveServoLeft(float setting) {
    byte cmd = SERVO_DRIVE_LEFT_PWM_STOP;
    float fcmd;
    if(setting > 0) {
        fcmd = LINSCALE(setting, 0.0, +1.0, SERVO_DRIVE_LEFT_PWM_STOP, SERVO_DRIVE_LEFT_PWM_MAX);
        cmd = round(fcmd);
    }
    else if (setting < 0) {
        fcmd = LINSCALE(setting, -1.0, 0.0, SERVO_DRIVE_LEFT_PWM_MIN, SERVO_DRIVE_LEFT_PWM_STOP);
        cmd = round(fcmd);
    }
    driveServoLeftCmd(cmd);
}

void driveServoRight(float setting) {
    byte cmd = SERVO_DRIVE_RIGHT_PWM_STOP;
    float fcmd;
    if(setting > 0) {
        fcmd = LINSCALE(setting, 0.0, +1.0, SERVO_DRIVE_RIGHT_PWM_STOP, SERVO_DRIVE_RIGHT_PWM_MAX);
        cmd = round(fcmd);
    }
    else if (setting < 0) {
        fcmd = LINSCALE(setting, -1.0, 0.0, SERVO_DRIVE_RIGHT_PWM_MIN, SERVO_DRIVE_RIGHT_PWM_STOP);
        cmd = round(fcmd);
    }
    driveServoRightCmd(cmd);    
}

/** Panning Servo **/
static float panning_setpoint;

void setPanningServo(float angle) {
    float fcmd = LINSCALE(angle, -90, +90, SERVO_PANNING_PWM_MIN, SERVO_PANNING_PWM_MAX);
    byte cmd = (byte) round(fcmd);
    analogWrite(SERVO_PANNING_PWM_PIN, cmd);
    panning_setpoint = angle;
}

float getPanningServoSetpoint() {
    return panning_setpoint;
}

unsigned long estimatePanningTime(float target_angle) {
    float arc = getShortestArc(panning_setpoint, target_angle);
    return (unsigned long) ceil(arc*SERVO_PANNING_SPEED);
}