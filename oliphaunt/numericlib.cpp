
#include "numericlib.h"
#include <Arduino.h>

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
    return last_value + timestep*last_diff/1000;
}

void EulerIntegrator::feedData(float differential, unsigned long update_time) {
    last_value = eulerStep(update_time - last_update_time);
    last_update_time = update_time;
    last_diff = differential;
}

float EulerIntegrator::getResult(unsigned long eval_time) {
    return eulerStep(eval_time - last_update_time);
}