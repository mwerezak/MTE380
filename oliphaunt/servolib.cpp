#include "servolib.h"
#include <Arduino.h>
#include "utility.h"

void driveServosStop() {
    analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, SERVO_DRIVE_LEFT_PWM_STOP);
    analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, SERVO_DRIVE_RIGHT_PWM_STOP);
}

void driveServosNeutral() {
    analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, SERVO_DRIVE_LEFT_PWM_NEUTRAL);
    analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, SERVO_DRIVE_RIGHT_PWM_NEUTRAL);
}

void driveServoLeftCmd(char command) {
    byte pwm = constrain(command, SERVO_DRIVE_LEFT_PWM_MIN, SERVO_DRIVE_LEFT_PWM_MAX);
    analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, pwm);
}

void driveServoRightCmd(char command) {
    byte pwm = constrain(command, SERVO_DRIVE_RIGHT_PWM_MIN, SERVO_DRIVE_RIGHT_PWM_MAX);
    analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, pwm);
}

void driveServoLeft(float setting) {
    char cmd = SERVO_DRIVE_LEFT_PWM_STOP;
    if(setting > 0) {
        cmd = (char) LINSCALE(setting, 0.0, +1.0, SERVO_DRIVE_LEFT_PWM_STOP, SERVO_DRIVE_LEFT_PWM_MAX);
    }
    else if (setting < 0) {
        cmd = (char) LINSCALE(setting, -1.0, 0.0, SERVO_DRIVE_LEFT_PWM_MIN, SERVO_DRIVE_LEFT_PWM_STOP);
    }
    driveServoLeftCmd(cmd);
}

void driveServoRight(float setting) {
    char cmd = SERVO_DRIVE_RIGHT_PWM_STOP;
    if(setting > 0) {
        cmd = (char) LINSCALE(setting, 0.0, +1.0, SERVO_DRIVE_RIGHT_PWM_STOP, SERVO_DRIVE_RIGHT_PWM_MAX);
    }
    else if (setting < 0) {
        cmd = (char) LINSCALE(setting, -1.0, 0.0, SERVO_DRIVE_RIGHT_PWM_MIN, SERVO_DRIVE_RIGHT_PWM_STOP);
    }
    driveServoRightCmd(cmd);    
}