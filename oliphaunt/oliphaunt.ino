
#include <L3G.h>
#include <Wire.h>

#include "actionqueue.h"
#include "movelib.h"
#include "trackinglib.h"

//#define PROCESS_DELAY 1000 //miliseconds

//ActionManager actionManager;

void setup() {
	Serial.begin(115200);
	Wire.begin();
	
	Serial.println("Initializing tracking...");
	initTracking();
	
	Serial.println("Initializing action queue...");
	initQueue();
	
}

void loop() {
	processMain();
	processTracking();
}