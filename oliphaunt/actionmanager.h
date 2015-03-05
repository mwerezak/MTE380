#ifndef _ACTIONMANAGER_H
#define _ACTIONMANAGER_H

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
        virtual bool checkFinished() { return TRUE; }
        
        // Called once, after the task has been determined to be finished.
        virtual void cleanup() {}
};

class ActionManager {
    private:
        Action* currentAction, nextAction;
    
    public:
        // Sets the specified task as the next one to execute after the current one is finished.
        // Actions that intend to have another task run after they are finished should use this.
        void setNext(Action *nextAction);
        
        // Immediately ends the current task and starts nextAction
        void forceNext(Action *nextAction);
        
        // Returns a pointer to the current active task.
        Action* getActiveAction();
        
        // Called by the main loop.
        void process();
}

#endif