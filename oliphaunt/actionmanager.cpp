
#include "actionmanager.h"

void ActionManager::process() {
    if(currentAction != NULL) {
        if(currentAction->checkFinished()) {
            killCurrentAction();
        } else {
            currentAction->doWork();
        }
    }
    
    //check currentAction again in case it was killed.
    if(currentAction == NULL) {
        Serial.println("startNextAction");
        startNextAction();
    }
}

void ActionManager::startNextAction() {
    if(nextAction == NULL) return;
    
    currentAction = nextAction;
    nextAction = NULL;
    
    currentAction->setup();
}

void ActionManager::killCurrentAction() {
    if(currentAction == NULL) return;
    
    currentAction->cleanup();
    currentAction = NULL;
}

void ActionManager::setNext(Action *next) {
    nextAction = next;
}

void ActionManager::forceNext(Action *next) {
    killCurrentAction();
    setNext(next);
    startNextAction();
}

Action* ActionManager::getActiveAction() {
    return currentAction;
}