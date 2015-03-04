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

long readSound(int);
bool readIRC(int);
bool startIRF(int);
float readIRF(int,int);
bool stopIRF(int);
