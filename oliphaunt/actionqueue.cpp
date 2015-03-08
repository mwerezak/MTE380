#include "actionqueue.h"

Action* currentAction;
int _startIdx, _endIdx; //use half-open interval convention
ActionQueueItem _actionQueue[ACTION_QUEUE_SIZE];

ActionQueueItem* _removeNext();
ActionQueueItem* _removeLast();

void _startNextAction();
void _killCurrentAction();

void initQueue() {
    _startIdx = 0;
    _endIdx = 1;
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
        Serial.println("startNextAction");
        _startNextAction();
    }
}

Action* getCurrentAction() {
    return currentAction;
}

//returns true if the queue is not empty
boolean queueIsEmpty() {
    if(_startIdx == ACTION_QUEUE_SIZE - 1)
        return (_endIdx == 0);
    return (_endIdx == _startIdx + 1);
}

boolean queueIsFull() {
    return (_startIdx == _endIdx);
}

//removes and returns the first item in the queue
ActionQueueItem* _removeNext() {
    if(queueIsEmpty()) return NULL;
    
    ActionQueueItem* removed = &_actionQueue[_startIdx];
    if(++_startIdx == ACTION_QUEUE_SIZE) _startIdx = 0;
    return removed;
}

//removes and returns the last item in the queue
ActionQueueItem* _removeLast() {
    if(queueIsEmpty()) return NULL;
    
    ActionQueueItem* removed = &_actionQueue[_endIdx];
    if(--_endIdx == 0) _endIdx = ACTION_QUEUE_SIZE - 1;
    return removed;
}

void _startNextAction() {
    ActionQueueItem* nextAction = _removeNext();
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

/** Queueing actions **/

//Forces the current action to stop, and starts next. Skips the queue.
void forceNextAction(Action* next, ActionArgs args) {
    _killCurrentAction();
    currentAction = next;
    currentAction->setup(&args);
}

//Adds to the front of the queue, essentially
void setNextAction(Action* next, ActionArgs args) {
    if(queueIsFull()) return;
    
    if(--_startIdx == 0) _startIdx = ACTION_QUEUE_SIZE - 1;
    
    _actionQueue[_startIdx].action = next;
    _actionQueue[_startIdx].args = args;
}

//Adds to the end of the queue
void queueAction(Action* action, ActionArgs args) {
    if(queueIsFull()) return;
    
    if(++_endIdx == ACTION_QUEUE_SIZE) _endIdx = 0;
    
    _actionQueue[_endIdx].action = action;
    _actionQueue[_endIdx].args = args;
}