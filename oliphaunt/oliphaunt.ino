
#include <L3G.h>
#include <Wire.h>

#include "movelib.h"
#include "actionqueue.h"
#include "gyro_driver.h"
#include "common.h"

//#define PROCESS_DELAY 1000 //miliseconds

//ActionManager actionManager;

void setup() {
	Serial.begin(115200);
	Wire.begin();
    
	Serial.println("Initializing gyro...");
	initGyro();
	
	Serial.println("Initializing action queue...");
	initQueue();
	
}

void loop() {
	processMain();
	updateGyro();
}