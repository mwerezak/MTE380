#include "servolib.h"
#include <Arduino.h>

void driveServosStop() {
    analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, SERVO_DRIVE_LEFT_PWM_STOP);
    analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, SERVO_DRIVE_RIGHT_PWM_STOP);
}

void driveServosNeutral() {
    analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, SERVO_DRIVE_LEFT_PWM_NEUTRAL);
    analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, SERVO_DRIVE_RIGHT_PWM_NEUTRAL);
}

void driveServoLeft(char command) {
    int pwm = SERVO_DRIVE_LEFT_PWM_STOP + command;
    if(pwm < SERVO_DRIVE_LEFT_PWM_MIN) pwm = SERVO_DRIVE_LEFT_PWM_MIN;
    if(pwm > SERVO_DRIVE_LEFT_PWM_MAX) pwm = SERVO_DRIVE_LEFT_PWM_MAX;
    analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, pwm);
}

void driveServoRight(char command) {
    int pwm = SERVO_DRIVE_RIGHT_PWM_STOP + command;
    if(pwm < SERVO_DRIVE_RIGHT_PWM_MIN) pwm = SERVO_DRIVE_RIGHT_PWM_MIN;
    if(pwm > SERVO_DRIVE_RIGHT_PWM_MAX) pwm = SERVO_DRIVE_RIGHT_PWM_MAX;
    analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, pwm);
}
