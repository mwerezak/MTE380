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

/*
 * Synopsis     : bool startIRF(int _PIN_ONE) *
 * Arguments    : int  _PIN_ONE : Digital input pin of long distance IR sensor
 *
 * Description  : enables the IR sensor to start reading
 * 
 * Returns      : bool: 1-started, 0 failed to start
 */
bool startDistanceIR(int _PIN_ONE){
    
    pinMode(_PIN_ONE,OUTPUT);
    digitalWrite(_PIN_ONE,HIGH);
    return false;

}//end startIRF

/*
 * Synopsis     : float readIRF(int _PIN_ONE, int _PIN_TWO) *
 * Arguments    : int  _PIN_ONE : Analogue output pin of long
                                  distance IR sensor
 *
 * Description  : returns distance from IR sensor in cm, Range 10-150 cm
 * 
 * Returns      : float: distnce in cm
 */
float readDistanceIR(int _PIN_ONE){
    
    int val;
    delayMicroseconds(22);
    val = analogRead(_PIN_ONE);
    delay(20);
    return (float)exp((1/(val * 5.0/1024.0)+1.723)/0.909);

}//end readIRF

/*
 * Synopsis     : bool stopIRF(int _PIN_ONE) *
 * Arguments    : int  _PIN_ONE : Digital input pin of long distance IR sensor
 *
 * Description  : stops IR sensor from reading data 
 *                WHEN AND WHY DO WE WANT TO CALL THIS?
 * 
 * Returns      : bool: 1-Stopped, 0-failed
 */
bool stopDistanceIR(int _PIN_ONE){

    digitalWrite(_PIN_ONE,LOW);
    return true; 

}//end stopIRF
