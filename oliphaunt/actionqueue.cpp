//#define DBG_ACTION_QUEUE

#include "actionqueue.h"
#include "utility.h"

Action *currentAction;

static int _startIdx, _queueLen;
static ActionQueueItem _actionQueue[ACTION_QUEUE_SIZE];
static unsigned long _last_process_time;

ActionQueueItem* _removeNext();
ActionQueueItem* _removeLast();
int _endIdx();

void _startAction(Action *action, ActionArgs *args);
void _startNextAction();

void initQueue() {    
    _startIdx = 0;
    _queueLen = 0;
    _last_process_time = millis();
}

void processMain() {
    unsigned long time = millis();
    if(currentAction && time >= _last_process_time + ACTION_PROCESS_DELAY) {
        if(currentAction->checkFinished()) {
            killCurrentAction();
        } else {
            currentAction->doWork();
        }
        _last_process_time = time;
    }
    
    //check currentAction again in case it was killed.
    if(!currentAction) {
        _startNextAction();
    }
    
    #ifdef DBG_ACTION_QUEUE
    PRINTF("current: %s, queuelen: %d", currentAction? currentAction->getName() : "None", queueLength());
    #endif
}

Action* getCurrentAction() {
    return currentAction;
}

//puts an action into the running state
void _startAction(Action *action, ActionArgs *args) {
    ProcessStatus status = action->getProcessStatus();
    if(status == STOPPED)
        action->start(args);
    else if(status == SUSPENDED)
        action->restart();
}

//pops the next action off the queue and starts it.
void _startNextAction() {
    ActionQueueItem *nextAction = _removeNext();
    if(nextAction) {
        currentAction = nextAction->action;
        _startAction(currentAction, &nextAction->args);
        #ifdef DBG_ACTION_QUEUE
        PRINTF("startNextAction: %s", currentAction? currentAction->getName(): "INVALID");
        #endif
    }
}

//Forces the current action to stop, and starts next. Skips the queue.
void forceNextAction(Action *next, ActionArgs *args) {
    killCurrentAction();
    currentAction = next;
    _startAction(currentAction, args);
}

Action* killCurrentAction() {
    if(!currentAction) return NULL;
    
    Action *stopped = currentAction;
    currentAction->stop();
    currentAction = NULL;
    return stopped;
}

Action* suspendCurrentAction() {
    if(!currentAction) return NULL;

    Action *suspended = currentAction;
    currentAction->suspend();
    currentAction = NULL;
    return suspended;
}

/** Queueing actions **/

//Gets the index of the last item in the queue
int _endIdx() {
    return (_startIdx + _queueLen - 1)%ACTION_QUEUE_SIZE;
}

//Adds to the front of the queue, essentially
void setNextAction(Action *next, ActionArgs *args) {
    if(queueIsFull()) {
        Serial.println("Action queue overrun!");
        while(1);
    }
    
    if(--_startIdx < 0) _startIdx = ACTION_QUEUE_SIZE - 1;
    _queueLen++;
    
    _actionQueue[_startIdx].action = next;
    if(args) _actionQueue[_startIdx].args = *args;
}

//Adds to the end of the queue
void queueAction(Action *action, ActionArgs *args) {
    if(queueIsFull()) {
        Serial.println("Action queue overrun!");
        while(1);
    }
    
    _queueLen++;
    
    int endIdx = _endIdx();
    _actionQueue[endIdx].action = action;
    if(args) _actionQueue[endIdx].args = *args;
}


//removes and returns the first item in the queue
//If anything is subsequently added to the queue the returned pointer may be invalidated!
ActionQueueItem* _removeNext() {
    if(queueIsEmpty()) return NULL;
    
    ActionQueueItem* removed = &_actionQueue[_startIdx];
    
    if(++_startIdx == ACTION_QUEUE_SIZE) _startIdx = 0;
    _queueLen--;
    
    return removed;
}

//removes and returns the last item in the queue
//If anything is subsequently added to the queue the returned pointer may be invalidated!
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
