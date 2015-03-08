#ifndef _ACTIONMANAGER_H
#define _ACTIONMANAGER_H

#include <Arduino.h>

//typedef enum { NONE=0, INT=1, LONG=2, FLOAT=3, UINT=4, ULONG=5 } ActionParamType;

typedef union ActionArgsUnion {
    int intval;
    long longval;
    float floatval;
    unsigned int uintval;
    unsigned long ulongval;
} ActionArg;

/*
typedef struct ActionParamStruct {
    ActionParamType type;
    ActionParamValue value;
} ActionParam;

const struct ActionParamStruct NoneParam = { .type = NONE };
*/

#define MAX_ACTION_ARGS 8
typedef struct ActionArgsStruct {
    ActionArg list[MAX_ACTION_ARGS];
} ActionArgs;

#define ARGS(ARG_STRUCT, IDX, ARG_FIELD) ARG_STRUCT.list[IDX].ARG_FIELD
#define ARGSP(ARG_STRUCT_PTR, IDX, ARG_FIELD) ARG_STRUCT_PTR->list[IDX].ARG_FIELD

/*
    Encapsulates an action that the robot needs to perform into a group of callbacks.
    Each of the public methods will be called by the ActionManager at the appropriate time.
*/
class Action {
    protected:
        //helper method to check parameters
        //boolean checkParam(ActionParams *params, int paramIdx, ActionParamType expected);
    
    public:
        //const char* name = "Action"; //for debugging
        
        // Called once, when the task is first made active.
        virtual void setup(ActionArgs *args) {}
        
        // Called every loop iteration while the task is active.
        virtual void doWork() {}
        
        // Called to check if the task is done
        virtual boolean checkFinished() { return true; }
        
        // Called once, after the task has been determined to be finished.
        virtual void cleanup() {}
};

/*
    Defines an Action type that has a pointer to a static instance.
*/
template<typename TDerivedAction>
class SingletonAction: public Action {
    public:
        static TDerivedAction* instance() {
            static TDerivedAction _instance;
            return &_instance;
        }
    
    protected:
        //static TDerivedAction _instance;
        SingletonAction() {}
    
    /*
    private:
        SingletonAction(SingletonAction const&) = delete;
        void operator=(SingletonAction const&) = delete;
    */
};

/*
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
*/
#endif