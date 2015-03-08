#ifndef _ACTIONS_DEF_H
#define _ACTIONS_DEF_H

/*
    Some crappy example actions.
*/
#include <Arduino.h>
#include "movelib.h"
#include "action.h"

class DriveAction : public SingletonAction<DriveAction> {
    public:
        char left, right;
        unsigned long end_at;
        
        /*
            Parameter 0: left
            Parameter 1: right
            Parameter 2: duration
        */
        void setup(ActionArgs *args) {
            //ActionArg* arglist = _args->list;
            unsigned long duration;
            
            left        = ARGSP(args, 0, intval);
            right       = ARGSP(args, 1, intval);
            duration    = ARGSP(args, 2, ulongval);
            
            analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, SERVO_DRIVE_LEFT_PWM_NEUTRAL + left);
            analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, SERVO_DRIVE_RIGHT_PWM_NEUTRAL + right);
            end_at += duration + millis();
        }
        
        boolean checkFinished() {
            Serial.print("checkFinished: ");
            Serial.print(millis());
            Serial.print(" >= ");
            Serial.println(end_at);
            return (millis() >= end_at);
        }
        
        //though we should still clean up in case someone forces us to stop
        void cleanup() {
            //digitalWrite(SERVO_DRIVE_CTL_PIN, LOW); //neutral the drive servos
            analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, 0);
            analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, 0);
        }
};
#endif