/*Sensor library
 *
 *Sound sensor read function*/
#include "slib.h"
 
long readSound(int _PIN) {
  pinMode(_PIN, OUTPUT);
  digitalWrite(_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(_PIN, LOW);
  pinMode(_PIN, INPUT);
  return (pulseIn(_PIN, HIGH)/29/2);
}

bool readIRC(int _PIN){
  return digitalRead(_PIN);
}

bool startIRF(int _PIN_ONE){
  pinMode(_PIN_ONE,OUTPUT);
  digitalWrite(_PIN_ONE,HIGH);
  return false;
}

float readIRF(int _PIN_ONE, int _PIN_TWO){
  delayMicroseconds(22);
  int val = analogRead(_PIN_TWO);
  delay(20);
  return (float)exp((1/(val * 5.0/1024.0)+1.723)/0.909);
}

bool stopIRF(int _PIN_ONE){
   digitalWrite(_PIN_ONE,LOW);
  return true; 
}
