#include "movelib.h"
#include "actionmanager.h"
#include "example_actions_def.h"

/*** MAKE SURE TO UPDATE MOVELIB.H BEFORE UPLOADING ***/

#define PROCESS_DELAY 10 //miliseconds

ActionManager actionManager;

//last minute queue
#define ACTION_QUEUE_SIZE 8
int counter;
DriveAction actionQueue[ACTION_QUEUE_SIZE];

void setup() {
	Serial.begin(9600);
	
/*	pinMode(SERVO_DRIVE_CTL_PIN, OUTPUT); */
	
	actionQueue[0] = DriveAction( 120,  120, 5000);
	actionQueue[1] = DriveAction(-120,  120, 5000);
	actionQueue[2] = DriveAction( 120,  120, 5000);
	actionQueue[3] = DriveAction( 120, -120, 5000);
	actionQueue[4] = DriveAction(-120, -120, 5000);
	actionQueue[5] = DriveAction( 120, -120, 5000);
	actionQueue[6] = DriveAction(-120, -120, 5000);
	actionQueue[7] = DriveAction(-120,  120, 5000);
	
	counter = 0;
	actionManager.forceNext(&actionQueue[0]);
	
	Serial.print("active: ");
	Serial.println((long)actionManager.getActiveAction());
}

void loop() {
	if(!actionManager.getActiveAction()) {
		counter++;
		actionManager.setNext(&actionQueue[counter % ACTION_QUEUE_SIZE]);
		Serial.print("counter: ");
		Serial.println(counter);
		Serial.print("active: ");
		Serial.println((long)actionManager.getActiveAction());
	}

	actionManager.process();

	delay(PROCESS_DELAY);
}