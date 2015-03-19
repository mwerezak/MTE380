#include <Servo.h>
#include "PINS.h"
#include "slib.h"

long a;
int time;
int stoptime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
//  pinMode(2, OUTPUT);
//  a=600;
//  time = 0;
//  stoptime = 0;
  //startIRF(IRF_PIN_O); 
  //pinMode(3,OUTPUT);
  //turn(1900);
  
  //metal servo
  /* analogWrite(3,80);
  delay(4000);
  analogWrite(3,230);
  delay(4000);
  analogWrite(3,155);
  delay(4000);
  analogWrite(3,0); */
  
  //continuous
  //600 microsecond is CW max speed
  //2400 microsecond is CCW min speed
  //1380 - 1470 Microsecond netural zone PWM 0 is good also
  /*warning kickback when turning motors off so */
  /* analogWrite(3,1); //CW M1: 1, M2: 1
  delay(4000);
  analogWrite(3,75); //slow speed CW M1: 176, M2:75 
  delay(4000);
  /* analogWrite(3,180); //neutral zone M1: 180, M2: 180
  delay(4000);
  analogWrite(3,188); //slow speed CCW M1: 188, M2: 188
  delay(4000);
  analogWrite(3,250); //CCW M1: 253, M2: 250
  delay(4000); 
  analogWrite(3,0);*/ //stop so is 255 but this one is recommended
  
}

void loop() {
  // put your main code here, to run repeatedly:
    /* Serial.println("Start - 150");
    analogWrite(3,150);
    delay(5000);
    Serial.println("255");
    analogWrite(3,255);
    delay(5000);
    Serial.println("0");
    analogWrite(3,0);
    delay(5000);
    Serial.println("180");
    analogWrite(3,180);
    delay(5000);
    Serial.println("End"); */
}

void turn(int pos){
  digitalWrite(2, HIGH);
  delayMicroseconds(pos); // Approximately 10% duty cycle @ 1KHz
  digitalWrite(2, LOW);
  delay(20);
}
