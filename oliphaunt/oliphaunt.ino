
#include <Wire.h>
#include <L3G.h>
#include <LSM303.h>

#include "actionqueue.h"
#include "movelib.h"
#include "trackinglib.h"

#include "action.h"

//#define PROCESS_DELAY 1000 //miliseconds

//ActionManager actionManager;

#include "servolib.h"

void setup() {
	Serial.begin(115200);
	Wire.begin();
	
	Serial.println("Initializing tracking...");
	initTracking();
	
	Serial.println("Initializing action queue...");
	initQueue();
	
	Serial.println("Initializing servo drivers...");
	initDriveServos();
	
	delay(1000);
	
	queueAction(TestDriveAction::instance(), NULL);
}

void loop() {
	processMain();
	processTracking();
	processDriveServos();
}