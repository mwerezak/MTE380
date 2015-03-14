#include "action.h"

void Action::start(ActionArgs *args) {        
    if(proc_status != STOPPED) return;
    
    proc_status = RUNNING;
    setup(args);
}

void Action::stop() {
    if(proc_status != RUNNING) return;
    
    cleanup();
    proc_status = STOPPED;
}

void Action::suspend() {
    if(proc_status != RUNNING) return;
    
    proc_status = SUSPENDED;
}

void Action::restart() {        
    if(proc_status != SUSPENDED) return;
    
    proc_status = RUNNING;
}