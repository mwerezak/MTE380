
#include <Wire.h>
#include <L3G.h>
#include <LSM303.h>

#include "actionqueue.h"
#include "movelib.h"
#include "trackinglib.h"

#include "action.h"

//#define PROCESS_DELAY 1000 //miliseconds

//ActionManager actionManager;

void setup() {
	Serial.begin(115200);
	Wire.begin();
	
	Serial.println("Initializing tracking...");
	initTracking();
	
	Serial.println("Initializing action queue...");
	initQueue();
	
	
	ActionArgs args;
	ARGS(args, 0, floatval) = 50.0;
	queueAction(TurnInPlaceToHeadingAction::instance(), &args);
	
	delay(1000);
}

void loop() {
	processMain();
	processTracking();
}