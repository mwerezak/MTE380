#ifndef _ACTIONS_DEF_H
#define _ACTIONS_DEF_H

/*
    Some crappy example actions.
*/
#include <Arduino.h>
#include "movelib.h"
#include "actionmanager.h"

class MoveForwardsAction : public Action {
    public:
        void setup() {
            digitalWrite(SERVO_DRIVE_CTL_PIN, HIGH); //enable drive servos
            analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, SERVO_DRIVE_LEFT_PWM_NEUTRAL + 100);
            analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, SERVO_DRIVE_RIGHT_PWM_NEUTRAL + 100);
        }
        
        boolean checkFinished() {
            return false; //don't stop, ever! - in reality we would check some sensors or something here
        }
        
        //though we should still clean up in case someone forces us to stop
        void cleanup() {
            digitalWrite(SERVO_DRIVE_CTL_PIN, LOW); //neutral the drive servos
            analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, 0);
            analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, 0);
        }
};

class TurnLeftAction : public Action {
    public:
        void setup() {
            digitalWrite(SERVO_DRIVE_CTL_PIN, HIGH); //enable drive servos
            analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, SERVO_DRIVE_LEFT_PWM_NEUTRAL + 100);
            analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, SERVO_DRIVE_RIGHT_PWM_NEUTRAL - 100);
        }
        
        boolean checkFinished() {
            return false; //don't stop, ever! - in reality we would check some sensors or something here
        }
        
        //though we should still clean up in case someone forces us to stop
        void cleanup() {
            digitalWrite(SERVO_DRIVE_CTL_PIN, LOW); //neutral the drive servos
            analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, 0);
            analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, 0);
        }
};

#endif