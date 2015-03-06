#include "movelib.h"
#include "actionmanager.h"
#include "example_actions_def.h"

/*** MAKE SURE TO UPDATE MOVELIB.H BEFORE UPLOADING ***/

//#define PROCESS_DELAY 2000 //microseconds
#define PROCESS_DELAY 50000 //microseconds - increased to 50 ms for the demo

ActionManager actionManager;

//last minute queue
#define ACTION_QUEUE_SIZE 8
int counter = 0;
DriveAction actionQueue[ACTION_QUEUE_SIZE];

void setup() {
	pinMode(SERVO_DRIVE_CTL_PIN, OUTPUT);
	
	actionQueue[0] = DriveAction( 100,  100, 5000);
	actionQueue[1] = DriveAction(-100,  100, 5000);
	actionQueue[2] = DriveAction( 100,  100, 5000);
	actionQueue[3] = DriveAction( 100, -100, 5000);
	actionQueue[4] = DriveAction(-100, -100, 5000);
	actionQueue[5] = DriveAction( 100, -100, 5000);
	actionQueue[6] = DriveAction(-100, -100, 5000);
	actionQueue[7] = DriveAction(-100,  100, 5000);
	
	actionManager.setNext(&actionQueue[0]);
}

void loop() {
	if(!actionManager.getActiveAction()) {
		counter++;
		actionManager.setNext(&actionQueue[counter % ACTION_QUEUE_SIZE]);
	}

	actionManager.process();
	delayMicroseconds(PROCESS_DELAY);
}