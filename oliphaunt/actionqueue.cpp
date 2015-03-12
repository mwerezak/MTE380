#include "actionqueue.h"

Action* currentAction;
int _startIdx, _queueLen;
ActionQueueItem _actionQueue[ACTION_QUEUE_SIZE];

ActionQueueItem* _removeNext();
ActionQueueItem* _removeLast();
int _endIdx();

void _startNextAction();
void _killCurrentAction();

void _debugQueue(char* message) {
    #ifdef AQUEUE_DBG
    Serial.println(message);
    #endif
}

void initQueue() {
    _debugQueue("initQueue()");
    
    _startIdx = 0;
    _queueLen = 0;
}

void processMain() {
    if(currentAction) {
        if(currentAction->checkFinished()) {
            _killCurrentAction();
        } else {
            currentAction->doWork();
        }
    }
    
    //check currentAction again in case it was killed.
    if(!currentAction) {
        _debugQueue("startNextAction");
        _startNextAction();
    }
}

Action* getCurrentAction() {
    return currentAction;
}

void _startNextAction() {
    ActionQueueItem *nextAction = _removeNext();
    if(nextAction) {
        currentAction = nextAction->action;
        currentAction->setup(&nextAction->args);
    }
}

void _killCurrentAction() {
    if(!currentAction) return;
    
    currentAction->cleanup();
    currentAction = NULL;
}

//Forces the current action to stop, and starts next. Skips the queue.
void forceNextAction(Action *next, ActionArgs *args) {
    _killCurrentAction();
    currentAction = next;
    currentAction->setup(args);
}

/** Suspending and Restarting **/

Action* suspendCurrentAction() {
    if(!currentAction) return NULL;

    Action *suspended = currentAction;
    currentAction = NULL;
    return suspended;
}

//Basically like forceNextAction but doesn't call setup.
//Will kill the current action unless it has been suspended first.
void restartAction(Action *action) {
    _killCurrentAction();
    currentAction = action;
}

/** Queueing actions **/

//Gets the index of the last item in the queue
int _endIdx() {
    return (_startIdx + _queueLen - 1)%ACTION_QUEUE_SIZE;
}

//Adds to the front of the queue, essentially
void setNextAction(Action *next, ActionArgs *args) {
    if(queueIsFull()) return;
    
    if(--_startIdx < 0) _startIdx = ACTION_QUEUE_SIZE - 1;
    _queueLen++;
    
    _actionQueue[_startIdx].action = next;
    _actionQueue[_startIdx].args = *args;
}

//Adds to the end of the queue
void queueAction(Action *action, ActionArgs *args) {
    if(queueIsFull()) return;
    
    _queueLen++;
    
    int endIdx = _endIdx();
    _actionQueue[endIdx].action = action;
    _actionQueue[endIdx].args = *args;
}


//removes and returns the first item in the queue
ActionQueueItem* _removeNext() {
    if(queueIsEmpty()) return NULL;
    
    ActionQueueItem* removed = &_actionQueue[_startIdx];
    
    if(++_startIdx == ACTION_QUEUE_SIZE) _startIdx = 0;
    _queueLen--;
    
    return removed;
}

//removes and returns the last item in the queue
ActionQueueItem* _removeLast() {
    if(queueIsEmpty()) return NULL;
    
    ActionQueueItem *removed = &_actionQueue[_endIdx()];
    _queueLen--;
    
    return removed;
}


//returns true if the queue is not empty
boolean queueIsEmpty() {
    return (_queueLen == 0);
}

boolean queueIsFull() {
    return (_queueLen == ACTION_QUEUE_SIZE);
}

//returns the number of items in the queue.
int queueLength() {
    return _queueLen;
}
