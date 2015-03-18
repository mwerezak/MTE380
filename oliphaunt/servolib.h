#ifndef _SERVOLIB_H
#define _SERVOLIB_H

#define SERVO_DRIVE_FULL_FORWARD        -1
#define SERVO_DRIVE_HALF_FORWARD        -1

#define SERVO_DRIVE_FULL_REVERSE        -1
#define SERVO_DRIVE_HALF_REVERSE        -1

// M1 - LEFT
#define SERVO_DRIVE_LEFT_PIN        11
#define SERVO_DRIVE_LEFT_NEUTRAL    0
#define SERVO_DRIVE_LEFT_STOP       180

#define SERVO_DRIVE_LEFT_FULL_FWD   253
#define SERVO_DRIVE_LEFT_HALF_FWD   188

#define SERVO_DRIVE_LEFT_FULL_REV   1
#define SERVO_DRIVE_LEFT_HALF_REV   176

// M2 - RIGHT
#define SERVO_DRIVE_RIGHT_PIN       12
#define SERVO_DRIVE_RIGHT_NEUTRAL   0
#define SERVO_DRIVE_RIGHT_STOP      180

#define SERVO_DRIVE_RIGHT_FULL_FWD  250
#define SERVO_DRIVE_RIGHT_HALF_FWD  188

#define SERVO_DRIVE_RIGHT_FULL_REV  1
#define SERVO_DRIVE_RIGHT_HALF_REV  75

// Panning Servo
#define SERVO_PANNING_PIN          10
#define SERVO_PANNING_MAX          255
#define SERVO_PANNING_MIN          0
#define SERVO_PANNING_EST_SPEED    2.833333 //ms/deg

//values mainly for debugging purposes
typedef enum { 
    FULL_FWD=2, 
    HALF_FWD=1, 
    HOLD_STOP=99, 
    HALF_REV=-1, 
    FULL_REV=-2, 
    NEUTRAL=0 
} DriveCmd;

void driveServosStop();
void driveServosNeutral();

void driveServoLeft(DriveCmd setting);
void driveServoRight(DriveCmd setting);

//Panning angles are specified as -90 to +90 deg
void setPanningServo(float target_angle);
float getPanningServoSetpoint();

//returns a duration in ms, 
//will NOT return a valid value if called immediately after setPanningServo()
unsigned long estimatePanningTime(float target_angle); 




#endif