#ifndef _SERVOLIB_H
#define _SERVOLIB_H

#define SERVO_DRIVE_FULL_FORWARD        70
#define SERVO_DRIVE_FULL_REVERSE        -170

// M1 - LEFT
#define SERVO_DRIVE_LEFT_PWM_PIN        11
#define SERVO_DRIVE_LEFT_PWM_NEUTRAL    0
#define SERVO_DRIVE_LEFT_PWM_STOP       180
#define SERVO_DRIVE_LEFT_PWM_MAX        253
#define SERVO_DRIVE_LEFT_PWM_MIN        1

// M2 - RIGHT
#define SERVO_DRIVE_RIGHT_PWM_PIN       12
#define SERVO_DRIVE_RIGHT_PWM_NEUTRAL   0
#define SERVO_DRIVE_RIGHT_PWM_STOP      180
#define SERVO_DRIVE_RIGHT_PWM_MAX       250
#define SERVO_DRIVE_RIGHT_PWM_MIN       1

// Panning Servo
#define SERVO_PANNNING_PWM_PIN          10


void driveServosStop();
void driveServosNeutral();

//takes a raw command, from MIN - MAX
void driveServoLeftCmd(char command);
void driveServoRightCmd(char command);

//takes a setting from +1.0 to -1.0
void driveServoLeft(float setting);
void driveServoRight(float setting);

void setPanningServo(float angle);
float getPanningServoSetpoint();





#endif