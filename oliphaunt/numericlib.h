#ifndef _NUMERICLIB_H
#define _NUMERICLIB_H

class NumericIntegrator {
public:
    virtual void reset(float init_value) = 0;
    
    // Adds a new data point to the integrator
    virtual void feedData(float update_diff, unsigned long update_time) = 0;
    
    // Evaluates the integrator at eval_time.
    // If feedData() has not yet been called, this must return the initial value.
    virtual float getResult(unsigned long eval_time) = 0;
};

class EulerIntegrator: public NumericIntegrator {
private:
    unsigned long last_update_time = 0;
    float last_diff = 0.0;
    double last_value = 0.0; //use extra precision for the accumulator
    
    double eulerStep(unsigned long timestep);
public:
    EulerIntegrator();
    
    virtual void reset(float init_value);
    virtual void feedData(float update_diff, unsigned long update_time);
    virtual float getResult(unsigned long eval_time);
};

#endif