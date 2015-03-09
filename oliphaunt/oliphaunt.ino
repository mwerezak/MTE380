#include "movelib.h"
#include "actionqueue.h"
#include "example_actions_def.h"

/*** MAKE SURE TO UPDATE MOVELIB.H BEFORE UPLOADING ***/

#define PROCESS_DELAY 1000 //miliseconds

//ActionManager actionManager;

void setup() {
	Serial.begin(9600);
    
	initQueue();
	
	ActionArgs forward;
	forward.list[0].intval = +120;
	forward.list[1].intval = +120;
	forward.list[2].ulongval = 5000;
	
	ActionArgs reverse;
	reverse.list[0].intval = -120;
	reverse.list[1].intval = -120;
	reverse.list[2].ulongval = 5000;
	
	ActionArgs left;
	left.list[0].intval = -120;
	left.list[1].intval = +120;
	left.list[2].ulongval = 5000;
	
	ActionArgs right;
	right.list[0].intval = +120;
	right.list[1].intval = -120;
	right.list[2].ulongval = 5000;
	
	queueAction(DriveAction::instance(), forward);
    queueAction(DriveAction::instance(), left);
	queueAction(DriveAction::instance(), forward);
	queueAction(DriveAction::instance(), right);
	queueAction(DriveAction::instance(), reverse);
	queueAction(DriveAction::instance(), right);
	queueAction(DriveAction::instance(), reverse);
	queueAction(DriveAction::instance(), left);
	//queueAction(DriveAction::instance(), reverse);
}

void loop() {
    #ifdef AQUEUE_DBG
    Serial.print("queue size: ");
    Serial.println(queueLength());
    #endif
    
	processMain();

	delay(PROCESS_DELAY);
}