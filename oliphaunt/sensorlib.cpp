/*Sensor library
 *
 *By: Group 9
 *2015
 *This code the property of group 9 and cannot be used unless with
 *the explicit permission of all of the group 9 members
 *
 *This library stores all the functions involved in retrieving data 
 *from the sensors directly.
 **/
#include "sensorlib.h"

#include <Arduino.h>

void initSensors() {
    //distance IR
    pinMode(DISTIR_CTL_PIN, OUTPUT);
    enableDistanceIR();
    
    //ultrasound
    pinMode(USOUND_DATCTL_PIN, INPUT); //might as well keep the usound pin in the high-impedance state
}

//note that the distance IR sensor requires 20 ms to stabilize after being enabled
void enableDistanceIR() {
    digitalWrite(DISTIR_CTL_PIN, HIGH);
}

void disableDistanceIR() {
    digitalWrite(DISTIR_CTL_PIN, LOW);
}

// Returns a distance in cm
float readDistanceIR(){
    
    int val1, val2, val3;
    delayMicroseconds(22);
    val1 = analogRead(SENSOR_IR_LR_PIN);
    val1 = (float)exp((1/(val1 * 5.0/1024.0)+1.723)/0.909);
    delay(22);
    val2 = analogRead(SENSOR_IR_LR_PIN);
    val2 = (float)exp((1/(val2 * 5.0/1024.0)+1.723)/0.909);
    if (fabs(val1-val2) > 2) {
        delay(22);
        val3 = analogRead(SENSOR_IR_LR_PIN);
        val3 = (float)exp((1/(val3 * 5.0/1024.0)+1.723)/0.909);
        if (fabs(val1-val3) < 2) {
            return (val1 + val3) / 2.0;
        } else if (fabs(val2-val3) < 2) {
            return (val2 + val3) / 2.0;
        } else {
            return (val1 + val2 + val3)/ 3.0;
        }
    }
    return (val1 - val2)/ 2.0;


// Returns a distance in cm
double readUltraSound() {
    unsigned long pulse_width;
    
    //command pulse
    pinMode(USOUND_DATCTL_PIN, OUTPUT);
    digitalWrite(USOUND_DATCTL_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(USOUND_DATCTL_PIN, HIGH);
    delayMicroseconds(USOUND_CTL_PULSE_WIDTH);
    digitalWrite(USOUND_DATCTL_PIN, LOW);
    
    //read output pulse
    pinMode(USOUND_DATCTL_PIN, INPUT);
    pulse_width = pulseIn(USOUND_DATCTL_PIN, HIGH, USOUND_TIMEOUT);
    
    if(!pulse_width) return -1.0; //timeout
    if(pulse_width >= USOUND_THRESHOLD) return USOUND_OUT_OF_RANGE;
    return pulse_width*USOUND_US_TO_CM;
}

bool getProxIRReading() {
    return digitalRead(PROXIR_PIN);
}
