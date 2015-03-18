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

#include "arduino.h"

#define SENSOR_SOUND_PIN		-1
#define SENSOR_IR_LR_PIN		-1
#define SENSOR_IR_SR_PIN		-1

long readSound();
bool readProxIR();
bool startDistanceIR();
float readDistanceIR();
bool stopDistanceIR();
