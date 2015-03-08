#ifndef _ACTIONS_DEF_H
#define _ACTIONS_DEF_H

/*
    Some crappy example actions.
*/
#include <Arduino.h>
#include "movelib.h"
#include "actionmanager.h"

class DriveAction : public Action {
    public:
        char left, right;
        unsigned long end_at;
        DriveAction(char _left=0, char _right=0, unsigned long duration=0) {
            left = _left; 
            right = _right; 
            end_at = duration;
        }
    
        void setup() {
            //digitalWrite(SERVO_DRIVE_CTL_PIN, HIGH); //enable drive servos
            analogWrite(SERVO_DRIVE_LEFT_PWM_PIN, SERVO_DRIVE_LEFT_PWM_NEUTRAL + left);
            analogWrite(SERVO_DRIVE_RIGHT_PWM_PIN, SERVO_DRIVE_RIGHT_PWM_NEUTRAL + right);
            end_at += millis();
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