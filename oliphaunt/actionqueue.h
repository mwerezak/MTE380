#include <Arduino.h>
#include "action.h"

#ifndef _ACTIONQUEUE_H
#define _ACTIONQUEUE_H

typedef struct ActionQueueItemStruct {
    Action* action;
    ActionArgs args;
} ActionQueueItem;

#define ACTION_QUEUE_SIZE 20

extern Action* currentAction;

void initQueue();
void processMain();
Action* getCurrentAction();
boolean queueIsEmpty();
boolean queueIsFull();
int queueLength();
void forceNextAction(Action* next, ActionArgs args);
void setNextAction(Action* next, ActionArgs args);
void queueAction(Action* action, ActionArgs args);

#endif