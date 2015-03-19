#ifndef _SENSORLIB_H
#define _SENSORLIB_H

#include <Arduino.h>

#define USOUND_DATCTL_PIN       22
#define USOUND_US_TO_CM         0.017241379310
#define USOUND_CTL_PULSE_WIDTH  10      //microseconds
#define USOUND_TIMEOUT          5200    //microseconds
#define USOUND_THRESHOLD        38000   //microseconds, the pulse width that indidcates no reading

//code using the ultrasound sensor should make sure to wait this time before calling readUltraSound() again
#define USOUND_CYCLE_TIME       50

#define PROXIR_PIN          24

#define DISTIR_CTL_PIN      48
#define DISTIR_DATA_PIN     2
#define DISTIR_REFRESH_TIME 25 //ms

void initSensors();

//note that the distance IR sensor requires 20 ms to stabilize after being enabled
void enableDistanceIR();
void disableDistanceIR();
double getDistanceIRReading(); //returns a distance in cm

double readUltraSound(); //returns a distance in cm
boolean getProxIRReading();

#endif