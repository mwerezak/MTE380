
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
	
	
	//ActionArgs args;
	//ARGS(args, 0, floatval) = 50.0;
	//queueAction(TurnInPlaceToHeadingAction::instance(), &args);
	
	driveServoLeft(FULL_FWD);
	delay(3000);
	driveServoLeft(HALF_FWD);
	delay(3000);
	driveServoLeft(HOLD_STOP);
	delay(3000);
	driveServoLeft(HALF_REV);
	delay(3000);
	driveServoLeft(FULL_REV);
	delay(3000);
	driveServoLeft(NEUTRAL);
	
	
	delay(1000);
}

void loop() {
	processMain();
	processTracking();
}