
#include "actionmanager.h"

void ActionManager::process() {
    if(currentAction) {
        if(currentAction->checkFinished()) {
            killCurrentAction();
        } else {
            currentAction->doWork();
        }
    }
    
    //check currentAction again in case it was killed.
    if(!currentAction) {
        startNextAction();
    }
}

void ActionManager::startNextAction() {
    if(!nextAction) return;
    
    currentAction = nextAction;
    nextAction = NULL;
    
    currentAction->setup();
}

void ActionManager::killCurrentAction() {
    if(!currentAction) return;
    
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