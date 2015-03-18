#ifndef _SENSORLIB_H
#define _SENSORLIB_H

#define USOUND_DATA_PIN 22
#define PROXIR_PIN 24
#define DISTIR_CTL_PIN 26
#define DISTIR_DATA_PIN 0

void initSensors();

//note that the distance IR sensor requires 20 ms to stabilize after being enabled
void enableDistanceIR();
void disableDistanceIR();
double getDistanceIRReading();

long readSound(int);
bool readProxIR(int);


#endif