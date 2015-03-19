/*slib.h*/

#include "arduino.h"

long readSound(int _PIN);
bool readIRC(int _PIN);
bool startIRF(int _PIN);
float readIRF(int _PIN_ONE,int _PIN_TWO);
bool stopIRF(int _PIN);
