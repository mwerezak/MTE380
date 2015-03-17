
//#define DBG_ABINTEGRATOR
//#define PERF_ABINTEGRATOR

#include "numericlib.h"
#include <Arduino.h>
#include "utility.h"

#define MSEC_TO_SEC 1000 //number of ms in a second

/** Euler Integrator **/

EulerIntegrator::EulerIntegrator() {
    reset(0);
}

void EulerIntegrator::reset(float init_value) {
    last_value = init_value;
    last_diff = 0;
    last_update_time = 0;
}

double EulerIntegrator::eulerStep(unsigned long timestep) {
    return last_value + timestep*last_diff/MSEC_TO_SEC;
}

void EulerIntegrator::feedData(float differential, unsigned long update_time) {
    last_value = eulerStep(update_time - last_update_time);
    last_update_time = update_time;
    last_diff = differential;
}

float EulerIntegrator::evalResult(unsigned long eval_time) {
    return eulerStep(eval_time - last_update_time);
}

float EulerIntegrator::getLastResult() {
    return last_value;
}

unsigned long EulerIntegrator::getLastUpdateTime() {
    return last_update_time;
}

/** Adams-Bashforth Integrator **/

AdamsBashforthIntegrator::AdamsBashforthIntegrator() {
    reset(0);
}

void AdamsBashforthIntegrator::reset(float init_value) {
    update_count = 0;
    last_value = init_value;
    for (int i = 0; i < ABI_BUF_SIZE; i++) {
        update_time[i] = 0;
        update_diff[i] = 0.0;
        beta[i] = 0.0;
    }
}

void AdamsBashforthIntegrator::feedData(float _update_diff, unsigned long _update_time) {
    #ifdef PERF_ABINTEGRATOR
    unsigned long timer = micros();
    #endif

    //update the accumulated value to the next step
    if(update_count >= ABI_BUF_SIZE) {
        last_value = adamsBashforthStep(_update_time - update_time[0]);
    }
    else {
        //if we don't have enough history for an AB step then resort to euler
        last_value = eulerStep(_update_time - update_time[0]);
        update_count++;
    }
    
    //push the new values onto the buffers
    pushArray<unsigned long>(update_time, _update_time);
    pushArray<float>(update_diff, _update_diff);
    
    
    #ifdef PERF_ABINTEGRATOR
    timer = micros() - timer;
    
    char str[80];
    snprintf(str, 80, "Evaluation took %hd microseconds.", timer);
    Serial.println(str);
    #endif
}

float AdamsBashforthIntegrator::getLastResult() {
    return last_value;
}

unsigned long AdamsBashforthIntegrator::getLastUpdateTime() {
    return update_time[0];
}

//Uses euler from the last update time to save on expensive calculation
float AdamsBashforthIntegrator::evalResult(unsigned long eval_time) {
    return eulerStep(eval_time - update_time[0]);
}

float AdamsBashforthIntegrator::eulerStep(unsigned long eval_timestep) {
    return last_value + eval_timestep*update_diff[0]/MSEC_TO_SEC;
}

float AdamsBashforthIntegrator::adamsBashforthStep(unsigned long eval_timestep) {
    calcBetas(eval_timestep);
    
    double update = eval_timestep*
        (
              beta[3]*update_diff[0]
            + beta[2]*update_diff[1]
            + beta[1]*update_diff[2]
            + beta[0]*update_diff[3]
        );
    
    //we handle time in u/ms, so convert back to s before evaluating
    return last_value + update/MSEC_TO_SEC;
}

// Pushes a new value onto the front of an array,
// shifting the array to the right and discarding the last element
template<typename T>
void AdamsBashforthIntegrator::pushArray(T array[], T new_val) {
    for(int i = ABI_BUF_SIZE-1; i > 0; i--) {
        array[i] = array[i-1]; //shift
    }
    array[0] = new_val;
}

//Does most of the heavy lifting
void AdamsBashforthIntegrator::calcBetas(unsigned long eval_timestep) {
    double num, den;
    double step[ABI_BUF_SIZE];
    step[0] = double(eval_timestep);
    step[1] = double(update_time[0] - update_time[1]);
    step[2] = double(update_time[1] - update_time[2]);
    step[3] = double(update_time[2] - update_time[3]);
    
    // calculate beta[0]
    num = 3*CUBE(step[0])
        + 4*SQR(step[0])*(3*step[1] + 2*step[2] + step[3])
        + 6*step[0]
        *(
              step[1]*(3*step[1] + 4*step[2] + 2*step[3])
            + step[2]*(step[2] + step[3])
         )
        + 12*step[1]
        *(
              step[1]*(step[1] + 2*step[2] + step[3])
            + step[2]*(step[2] + step[3])
         );
    
    den = 12*step[1]*(step[1] + step[2])*(step[1]+step[2]+step[3]);
    
    beta[0] = num/den;
    
    // calculate beta[1]
    num = 3*SQR(step[0])
        + 4*step[0]*(2*step[1] + 2*step[2] + step[3])
        + 6*step[1]*(step[1] + 2*step[2] + step[3])
        + 6*step[2]*(step[2] + step[3]);
        
    den = 12*step[1]*step[2]*(step[2] + step[3]);
    beta[1] = -num*step[0]/den;
    
    // calculate beta[2]
    num = 3*SQR(step[0])
        + 4*step[0]*(2*step[1] + step[2] + step[3])
        + 6*step[1]*(step[1] + step[2] + step[3]);
    
    den = 12*step[2]*step[3]*(step[1] + step[2]);
    
    beta[2] = num*step[0]/den;
    
    // calculate beta[3]
    num = 3*SQR(step[0])
        + 4*step[0]*(2*step[1] + step[2])
        + 6*step[1]*(step[1] + step[2]);
    
    den = 12*step[3]*(step[2] + step[3])*(step[1] + step[2] + step[3]);
    
    beta[3] = -num*step[0]/den;

    #ifdef DBG_ABINTEGRATOR
    printArray<double>(step, ABI_BUF_SIZE, "step");
    printArray<double>(beta, ABI_BUF_SIZE, "beta");
    #endif
}