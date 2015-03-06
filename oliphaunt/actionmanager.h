#ifndef _ACTIONMANAGER_H
#define _ACTIONMANAGER_H

#include <Arduino.h>

/*
    Encapsulates an action that the robot needs to perform into a group of callbacks.
    Each of the public methods will be called by the ActionManager at the appropriate time.
*/
class Action {
    public:
        // Called once, when the task is first made active.
        virtual void setup() {}
        
        // Called every loop iteration while the task is active.
        virtual void doWork() {}
        
        // Called to check if the task is done
        virtual boolean checkFinished() { return true; }
        
        // Called once, after the task has been determined to be finished.
        virtual void cleanup() {}
};


// This probably doesn't actually need to be a class, but whatever.
class ActionManager {
    private:
        Action *currentAction, *nextAction;
        
        void startNextAction();
        void killCurrentAction();
        
    public:
        // Ctor
        ActionManager() {
            currentAction = NULL;
            nextAction = NULL;
        }
    
        // Sets the specified action as the next one to execute after the current one is finished.
        // Actions that intend to have another action run after they are finished should use this.
        void setNext(Action *next);
        
        // Immediately ends the current action and starts the specified next one
        void forceNext(Action *next);
        
        // Returns a pointer to the current active action.
        Action* getActiveAction();
        
        // Called by the main loop.
        void process();
};

#endif