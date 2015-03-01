/*
    WIP servo system interface
    NOT TESTED YET
*/

#include "arduino_system.h"

#ifndef _SERVOS_H
#define _SERVOS_H

//Drive Servos
#define SERVO_DRIVE_CTL_PIN     -1  //TODO

#define SERVO_DRIVEL_PWM_PIN    -1  //TODO
#define SERVO_DRIVEL_PWM_NEUTRAL 0  //TODO

#define SERVO_DRIVER_PWM_PIN    -1  //TODO
#define SERVO_DRIVER_PWM_NEUTRAL 0  //TODO

//Lip and Capture Arm Servos
#define SERVO_CAPLIP_CTL_PIN    -1  //TODO

#define SERVO_LIP_PWM_PIN       -1  //TODO
#define SERVO_LIP_PWM_NEUTRAL    0  //TODO

#define SERVO_CAPARM_PWM_PIN    -1  //TODO
#define SERVO_CAPARM_PWM_NEUTRAL 0  //TODO

//Panning Servo
#define SERVO_PAN_CTL_PIN       -1  //TODO
#define SERVO_PAN_PWM_PIN       -1  //TODO
#define SERVO_PAN_PWM_NEUTRAL    0  //TODO

//table to track which 


//encapsulates a servo drive's state
class ServoDrive {
    byte neutral;
    byte cur_set;
};

#define SERVO_SET_MAX 2
class ServoDriveSet {

};

//SetSpeed
//GetSpeed
//AdjustSpeed

//SetEngaged
//IsEngaged

#endif //_SERVOS_H