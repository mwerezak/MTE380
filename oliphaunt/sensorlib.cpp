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

/*
 * Name         : readSound
 *
 * Synopsis     : long readSound(int _PIN)   *
 * Arguments    : None
 *
 * Description  : returns distance value from Sonar sensor in cm
 * 
 * Returns      : long: distance in cm
 */
long readSound(){

    pinMode(SENSOR_SOUND_PIN, OUTPUT);
    digitalWrite(SENSOR_SOUND_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(SENSOR_SOUND_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(SENSOR_SOUND_PIN, LOW);
    pinMode(SENSOR_SOUND_PIN, INPUT);
    return (pulseIn(SENSOR_SOUND_PIN, HIGH)/29/2);
    
}//end readSound

/*
 * Name         : readIRC
 *
 * Synopsis     : bool readIRC(int _PIN) *
 * Arguments    : None
 *
 * Description  : returns whether something is within 10 cm from the
                  IR sensor
 * 
 * Returns      : bool:  1 no, 0 yes
 */
bool readProxIR(){
    
    return digitalRead(SENSOR_IR_SR_PIN);
    
}//end readIRC

/*
 * Name         : startIRF
 *
 * Synopsis     : bool startIRF(int _PIN_ONE) *
 * Arguments    : None
 *
 * Description  : enables the IR sensor to start reading
 * 
 * Returns      : bool: 1-started, 0 failed to start
 */
bool startDistanceIR(){

    pinMode(SENSOR_IR_LR_PIN,OUTPUT);
    digitalWrite(SENSOR_IR_LR_PIN,HIGH);
    return false;

}//end startIRF

/*
 * Name         : readIRF
 *
 * Synopsis     : float readIRF(int _PIN_ONE, int _PIN_TWO) *
 * Arguments    : None
 *
 * Description  : returns distance from IR sensor in cm, Range 10-150 cm
 * 
 * Returns      : float: distnce in cm
 */
float readDistanceIR(){
    
    int val;
    delayMicroseconds(22);
    val = analogRead(SENSOR_IR_LR_PIN);
    delay(20);
    return (float)exp((1/(val * 5.0/1024.0)+1.723)/0.909);

}//end readIRF

/*
 * Name         : stopIRF
 *
 * Synopsis     : bool stopIRF(int _PIN_ONE) *
 * Arguments    : None
 *
 * Description  : stops IR sensor from reading data 
 *                WHEN AND WHY DO WE WANT TO CALL THIS?
 * 
 * Returns      : bool: 1-Stopped, 0-failed
 */
bool stopDistanceIR(){

    digitalWrite(SENSOR_IR_LR_PIN,LOW);
    return true; 

}//end stopIRF
