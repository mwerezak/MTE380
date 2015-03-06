#ifndef _TASKS_H
#define _TASKS_H

/*
    Support for intermittent tasks.
    A task is a callback that is executed repeatedly every N microseconds.
*/

typedef void(*TaskCallbackPtr)(unsigned long timeMilis, unsigned long timeMicros);
typedef unsigned long CallbackID;

/* Linked list node for storing callback info. */
typedef struct CallbackListNode {
    TaskCallbackPtr callback;
    unsigned long interval; //interval between calls in microseconds
    
    *CallbackListNode nextCallback;
} *CallbackListPtr;

static CallbackListPtr callbackList;

// Adds a callback to the list of callbacks to execute.
// Returns a unique callback ID.
CallbackID addTaskCallback(TaskCallbackPtr callback, unsigned long interval);

// Removes a callback
void removeTaskCallback(CallbackID id);

// Should be called by the main loop.
void processCallbacks();

#endif