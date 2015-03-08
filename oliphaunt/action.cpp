
#include "action.h"

/*
boolean Action::checkParam(ActionParams *params, int paramIdx, ActionParamType expected) {
    if(params->plist[paramIdx].type != expected) {
        Serial.print("ERROR setting up action ");
        Serial.print(this->name);
        Serial.print(": bad parameter type for parameter ");
        Serial.print(paramIdx);
        Serial.print(" -> got ");
        Serial.print(params->plist[paramIdx].type);
        Serial.print(", expected ");
        Serial.println(expected);
        return false;
    }
    return true;
}
*/




/*

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
*/