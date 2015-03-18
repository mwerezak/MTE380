#include <Arduino.h>

#include "action.h"

#ifndef _ACTIONQUEUE_H
#define _ACTIONQUEUE_H

#define ACTION_QUEUE_SIZE 20

typedef struct ActionQueueItemStruct {
    Action *action;
    ActionArgs args;
} ActionQueueItem;

/*
    Note: Actions themselves are not stored in the queue, only pointers.
    Memory and lifetime of Actions should be handled elsewhere.
    ActionArgs are however stored in the queue.
*/

extern Action *currentAction;

void initQueue();
void processMain();
Action* getCurrentAction();
boolean queueIsEmpty();
boolean queueIsFull();

//number of items in the queue, does not count the current action
int queueLength();

//Forces the current action to stop, and starts the given one, skipping the queue.
//Will kill the current action unless it has been suspended first.
void forceNextAction(Action *next, ActionArgs *args);

//Adds to the front of the queue, essentially
void setNextAction(Action *next, ActionArgs *args);

//Adds to the end of the queue
void queueAction(Action *action, ActionArgs *args);

//Returns the action that has been suspended. 
Action* suspendCurrentAction();

//Returns the action that has been stopped.
Action* killCurrentAction();

#endif