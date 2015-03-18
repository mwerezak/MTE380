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
    
}

//note that the distance IR sensor requires 20 ms to stabilize after being enabled
void enableDistanceIR() {
    digitalWrite(DISTIR_CTL_PIN, HIGH);
}

void disableDistanceIR() {
    digitalWrite(DISTIR_CTL_PIN, LOW);
}

double getDistanceIRReading() {
    long val = analogRead(DISTIR_DATA_PIN);
    return exp((1.0/(val * 5.0/1024.0)+1.723)/0.909);
}


/*
 * Synopsis     : long readSound(int _PIN)   *
 * Arguments    : int  _PIN : Digital pin number of Sonar sensor
 *
 * Description  : returns distance value from Sonar sensor in cm
 * 
 * Returns      : long: distance in cm
 */
long readSound(int _PIN){
    
    pinMode(_PIN, OUTPUT);
    digitalWrite(_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(_PIN, LOW);
    pinMode(_PIN, INPUT);
    return (pulseIn(_PIN, HIGH)/29/2);
    
}//end readSound

/*
 * Synopsis     : bool readIRC(int _PIN) *
 * Arguments    : int  _PIN : Digital pin number of proximity IR sensor
 *
 * Description  : returns whether something is within 10 cm from the
                  IR sensor
 * 
 * Returns      : bool:  1 no, 0 yes
 */
bool readProxIR(int _PIN){
    
    return digitalRead(_PIN);
    
}//end readIRC
