#ifndef _ACTIONMANAGER_H
#define _ACTIONMANAGER_H

#include <Arduino.h>

typedef union ActionArgsUnion {
    int intval;
    long longval;
    float floatval;
    unsigned int uintval;
    unsigned long ulongval;
} ActionArg;

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
    public:
        // For debugging
        virtual char* getName() { return "Action"; }
        
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
        SingletonAction() {}
    
    /* Uncomment this to prevent unwanted copying
    private:
        SingletonAction(SingletonAction const&) = delete;
        void operator=(SingletonAction const&) = delete;
    */
};

#endif