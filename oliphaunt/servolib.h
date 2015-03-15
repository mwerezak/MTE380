#ifndef _SERVOLIB_H
#define _SERVOLIB_H

//M1 - LEFT
#define SERVO_DRIVE_LEFT_PWM_PIN        11
#define SERVO_DRIVE_LEFT_PWM_NEUTRAL    0
#define SERVO_DRIVE_LEFT_PWM_STOP       180
#define SERVO_DRIVE_LEFT_PWM_MAX        253
#define SERVO_DRIVE_LEFT_PWM_MIN        1

//M2 - RIGHT
#define SERVO_DRIVE_RIGHT_PWM_PIN       12
#define SERVO_DRIVE_RIGHT_PWM_NEUTRAL   0
#define SERVO_DRIVE_RIGHT_PWM_STOP      180
#define SERVO_DRIVE_RIGHT_PWM_MAX       250
#define SERVO_DRIVE_RIGHT_PWM_MIN       1



void driveServosStop();
void driveServosNeutral();
void driveServoLeft(char command);
void driveServoRight(char command);






#endif