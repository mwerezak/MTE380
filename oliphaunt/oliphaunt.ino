
#include <Wire.h>
#include <L3G.h>
#include <LSM303.h>

#include "actionqueue.h"
#include "movelib.h"
#include "trackinglib.h"
#include "sensorlib.h"
#include "servolib.h"

//#define PROCESS_DELAY 1000 //miliseconds

//ActionManager actionManager;

#include "action.h"
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
	
	float angles[] = {60, 120, 60, 0, -60, -120, -60, 0, 60, 120, 60, 0};
	for(int i = 0; i < 12; i++) {
		ARGS(args, 0, floatval) = angles[i];
		queueAction(TurnInPlaceToHeadingAction::instance(), &args);
		queueAction(WaitAction::instance(), &wait_args);
	}
	*/
	
	/*
	ActionArgs args;
	ARGS(args, 0, floatval) = 0.0;
	ARGS(args, 1, floatval) = 100.0;
	ARGS(args, 2, floatval) = 10.0;
	queueAction(DriveToLocationAction::instance(), &args);
	queueAction(WaitAction::instance(), &wait_args);
	*/
	
	/*
	ActionArgs args;

	ARGS(args, 0, floatval) = -80;
	queueAction(PanShovelServo::instance(), &args);
	
	ARGS(args, 0, ulongval) = 2000;
	queueAction(WaitAction::instance(), &args);
	
	ARGS(args, 0, floatval) = 40.0;
	queueAction(PanShovelServo::instance(), &args);
	
	ARGS(args, 0, ulongval) = 2000;
	queueAction(WaitAction::instance(), &args);
	
	ARGS(args, 0, floatval) = 0;
	queueAction(PanShovelServo::instance(), &args);
	*/

	
	//measureSpeedChange(30000);
	
	
	//ARGS(args, 0, floatval) = 50.0;
	//queueAction(DriveToLocationAction::instance(), &args);
	
	//queueAction(TestDriveAction::instance(), NULL);
	
/*
	ActionArgs args;
	ARGS(args, 0, floatval) = 0.0;
	ARGS(args, 1, floatval) = 100.0;
	ARGS(args, 2, floatval) = 10.0;
	queueAction(DriveToLocationAction::instance(), &args);
	queueAction(WaitAction::instance(), &wait_args);
	
	
	ARGS(args, 0, floatval) = 100.0;
	ARGS(args, 1, floatval) = 100.0;
	queueAction(DriveToLocationAction::instance(), &args);
	queueAction(WaitAction::instance(), &wait_args);
	
	ARGS(args, 0, floatval) = 100.0;
	ARGS(args, 1, floatval) = 0.0;
	queueAction(DriveToLocationAction::instance(), &args);
	queueAction(WaitAction::instance(), &wait_args);
	
	ARGS(args, 0, floatval) = 0.0;
	ARGS(args, 1, floatval) = 0.0;
	queueAction(DriveToLocationAction::instance(), &args);
	queueAction(WaitAction::instance(), &wait_args);
	
	*/
	
	//startCapture();
	
	
	//setShovelServo(0.0);
	//setScoopServo(180.0);
	setPanningServo(SERVO_PANNING_MAX_ANGLE);
	
	delay(1000);
	
}

float angle = -30;

#include "sensorlib.h"

void loop() {
	processMain();
	processTracking();
	processDriveServos();
	
	
	if(!queueLength()){
		setPanningServo(angle);
		angle += 1;
		
		ActionArgs wait_args;
		ARGS(wait_args, 0, ulongval) = 100;
		setNextAction(WaitAction::instance(), &wait_args);
	}
	
	
	/*
	float reading = readUltraSound();
	Serial.print("sound: ");
	Serial.print(reading);
	
	reading = getDistanceIRReading();
	Serial.print(", distir: ");
	Serial.println(reading);
	*/
}