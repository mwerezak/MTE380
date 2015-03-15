#include "movelib.h"
#include "actionqueue.h"

/*** MAKE SURE TO UPDATE MOVELIB.H BEFORE UPLOADING ***/

#define PROCESS_DELAY 1000 //miliseconds

//ActionManager actionManager;

void setup() {
	Serial.begin(9600);
    
	initQueue();
	
}

void loop() {
    #ifdef AQUEUE_DBG
    Serial.print("queue size: ");
    Serial.println(queueLength());
    #endif
    
	processMain();

	delay(PROCESS_DELAY);
}