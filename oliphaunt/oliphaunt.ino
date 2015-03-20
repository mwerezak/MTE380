
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

ActionArgs wait_args;

void setup() {
	Serial.begin(115200);
	Wire.begin();
	
	Serial.println("Initializing sensor drivers...");
	initSensors();
	
	Serial.println("Initializing tracking...");
	initTracking();
	//releaseGyro();
	
	Serial.println("Initializing servo drivers...");
	initDriveServos();
	
	Serial.println("Initializing action queue...");
	initQueue();
	
	ARGS(wait_args, 0, ulongval) = 3000;
	
	//releaseGyro();
	
	/*
	float f;
	boolean b;
	
	Serial.println("Initializing action queue...");
	initQueue();
	
	float f;
	boolean b;
	
	ActionArgs args;

	ARGS(args, 0, floatval) = 0.0;
	queueAction(PanIRServo::instance(), &args);
	
	queueAction(WaitAction::instance(), &wait_args);
	
	ARGS(args, 0, floatval) = -90.0;
	ARGS(args, 1, floatval) = 90.0;
	ARGS(args, 2, floatval) = 0.0;
	ARGS(args, 3, floatval) = 10.0;
	ARGS(args, 4, floatval) = 0.1;
	ARGS(args, 5, ptrval) = &f;
	ARGS(args, 6, ptrval) = &b;
	queueAction(IRSweepForTarget::instance(), &args);
	*/
	
	
	/*
	ActionArgs args;
	
	ARGS(args, 0, floatval) = 45;
	queueAction(TurnInPlaceToHeadingAction::instance(), &args);
	
	queueAction(WaitAction::instance(), &wait_args);
	
	ARGS(args, 0, floatval) = 270;
	queueAction(TurnInPlaceToHeadingAction::instance(), &args);
	
	queueAction(WaitAction::instance(), &wait_args);
	
	ARGS(args, 0, floatval) = 135;
	queueAction(TurnInPlaceToHeadingAction::instance(), &args);
	
	queueAction(WaitAction::instance(), &wait_args);
	
	ARGS(args, 0, floatval) = 0;
	queueAction(TurnInPlaceToHeadingAction::instance(), &args);
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
	measureSpeedChange(30000);
	
	
	//ARGS(args, 0, floatval) = 50.0;
	//queueAction(DriveToLocationAction::instance(), &args);
	
	//queueAction(TestDriveAction::instance(), NULL);
	
	//driveServoLeft(FULL_FWD);
	//driveServoRight(FULL_FWD);
}

float angle = 0;

void loop() {
	processMain();
	processTracking();
	processDriveServos();
	
	/*
	if(!queueLength()){
		setScoopServo(angle);
		angle += 0.5;
		queueAction(WaitAction::instance(), &wait_args);
	}
	*/
}