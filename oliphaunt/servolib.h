#ifndef _SERVOLIB_H
#define _SERVOLIB_H

#define SERVO_DRIVE_DEADTIME        200 //ms

// M1 - LEFT
#define SERVO_DRIVE_LEFT_PIN        12
#define SERVO_DRIVE_LEFT_NEUTRAL    0
#define SERVO_DRIVE_LEFT_STOP       180

#define SERVO_DRIVE_LEFT_FULL_FWD   253
#define SERVO_DRIVE_LEFT_HALF_FWD   188

#define SERVO_DRIVE_LEFT_FULL_REV   1
#define SERVO_DRIVE_LEFT_HALF_REV   176

// M2 - RIGHT
#define SERVO_DRIVE_RIGHT_PIN       13
#define SERVO_DRIVE_RIGHT_NEUTRAL   0
#define SERVO_DRIVE_RIGHT_STOP      180

#define SERVO_DRIVE_RIGHT_FULL_FWD  1
#define SERVO_DRIVE_RIGHT_HALF_FWD  75

#define SERVO_DRIVE_RIGHT_FULL_REV  250
#define SERVO_DRIVE_RIGHT_HALF_REV  190     //188

// Panning Servo
#define SERVO_PANNING_PIN          9
#define SERVO_PANNING_MIN          60
#define SERVO_PANNING_MAX          155
#define SERVO_PANNING_EST_SPEED    2.833333 //ms/deg
#define SERVO_PANNING_MIN_ANGLE    -30
#define SERVO_PANNING_MAX_ANGLE    +30

// Scoop Servo
#define SERVO_SCOOP_PIN             11
#define SERVO_SCOOP_MIN             50
#define SERVO_SCOOP_MAX             245
#define SERVO_SCOOP_MIN_ANGLE       0
#define SERVO_SCOOP_MAX_ANGLE       180

// Shovel Servo
#define SERVO_SHOVEL_PIN             10
#define SERVO_SHOVEL_MIN             80
#define SERVO_SHOVEL_MAX             230
#define SERVO_SHOVEL_MIN_ANGLE       -80
#define SERVO_SHOVEL_MAX_ANGLE       40

//values mainly for debugging purposes
typedef enum { 
    FULL_FWD=2, 
    HALF_FWD=1, 
    HOLD_STOP=99, 
    HALF_REV=-1, 
    FULL_REV=-2, 
    NEUTRAL=0 
} DriveCmd;

void initDriveServos();
void processDriveServos();

void driveServosStop();
void driveServosNeutral();

void driveServoLeft(DriveCmd setting);
void driveServoRight(DriveCmd setting);

//Panning angles are specified as -90 to +90 deg
void setPanningServo(float target_angle);
float getPanningServoSetpoint();
void tuckPanningServo();

//returns a duration in ms, 
//will NOT return a valid value if called immediately after setPanningServo()
unsigned long estimatePanningTime(float target_angle); 

void setScoopServo(float target_angle);
void setShovelServo(float target_angle);


#endif