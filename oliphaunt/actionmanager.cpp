
#include "actionmanager.h"

ActionManager::process() {
    if(currentAction) {
        if(currentAction->checkFinished()) {
            killCurrentAction();
        } else {
            currentAction->doWork()
        }
    }
    
    //check currentAction again in case it was killed.
    if(!currentAction) {
        startNextAction();
    }
}

ActionManager::startNextAction() {
    if(!nextAction) return;
    
    currentAction = nextAction;
    nextAction = NULL;
    
    currentAction->setup();
}

ActionManager::killCurrentAction() {
    if(!currentAction) return;
    
    currentAction->cleanup();
    currentAction = NULL;
}

ActionManager::setNext(Action *next) {
    nextAction = next;
}

ActionManager::forceNext(Action *next) {
    killCurrentAction();
    setNext(next);
    startNextAction();
}