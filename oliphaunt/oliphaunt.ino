
#include <Wire.h>
#include <L3G.h>
#include <LSM303.h>

#include "actionqueue.h"
#include "movelib.h"
#include "trackinglib.h"
#include "sensorlib.h"

#include "action.h"

//#define PROCESS_DELAY 1000 //miliseconds

//ActionManager actionManager;

#include "servolib.h"
#include "sensor_actions.h"
#include "utility.h"
#include "capture.h"

void setup() {
	Serial.begin(115200);
	Wire.begin();
	
	Serial.println("Initializing sensor drivers...");
	initSensors();
	
	//Serial.println("Initializing tracking...");
	//initTracking();
	
	Serial.println("Initializing servo drivers...");
	initDriveServos();
	
	Serial.println("Initializing action queue...");
	initQueue();
	
	float f;
	boolean b;
	
	ActionArgs args;
	ARGS(args, 0, floatval) = 0.0;
	queueAction(PanIRServo::instance(), &args);
	
	/*
	ActionArgs args;
	ARGS(args, 0, floatval) = -50.0;
	ARGS(args, 1, floatval) = 50.0;
	ARGS(args, 2, floatval) = 0.0;
	ARGS(args, 3, floatval) = 10.0;
	ARGS(args, 4, floatval) = 0.1;
	ARGS(args, 5, ptrval) = &f;
	ARGS(args, 6, ptrval) = &b;
	queueAction(IRSweepForTarget::instance(), &args);
	*/
	
	/*
	ActionArgs args;
	ARGS(args, 0, floatval) = 0;
	queueAction(PanScoopServo::instance(), &args);
	
	ARGS(args, 0, ulongval) = 2000;
	queueAction(WaitAction::instance(), &args);
	
	ARGS(args, 0, floatval) = 180.0;
	queueAction(PanScoopServo::instance(), &args);
	
	ARGS(args, 0, ulongval) = 2000;
	queueAction(WaitAction::instance(), &args);
	
	ARGS(args, 0, floatval) = 0;
	queueAction(PanScoopServo::instance(), &args);
	*/
	
	delay(1000);
}

void loop() {
	processMain();
	processTracking();
	processDriveServos();
}