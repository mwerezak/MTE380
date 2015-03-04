#include <Servo.h>
#include "PINS.h"
#include "slib.h"

long a;
int time;
int stoptime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
//  pinMode(2, OUTPUT);
//  a=600;
//  time = 0;
//  stoptime = 0;
  startIRF(IRF_PIN_O); 
}

void loop() {
  // put your main code here, to run repeatedly:
//  for(int b=600;b<=2400;b=b+10){
//   time =millis();
//   stoptime = time+abs(a-b);
//   while(millis()<stoptime){
//    turn(b);
//   }
//   a = b;
//  }
  a =readIRF(IRF_PIN_O,IRF_PIN_I);
   Serial.println(a);
}

void turn(int pos){
  digitalWrite(2, HIGH);
  delayMicroseconds(pos); // Approximately 10% duty cycle @ 1KHz
  digitalWrite(2, LOW);
  delay(20);
}
