#include "movelib.h"
#include "actionmanager.h"
#include "example_actions_def.h"

#define PROCESS_DELAY 2000 //microseconds

ActionManager actionManager;
TurnLeftAction turnLeft;

void setup() {
	pinMode(SERVO_DRIVE_CTL_PIN, OUTPUT);
	
	actionManager.setNext(&turnLeft);
}

void loop() {
	actionManager.process();
	delayMicroseconds(PROCESS_DELAY);
}