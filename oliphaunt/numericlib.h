#ifndef _NUMERICLIB_H
#define _NUMERICLIB_H

class NumericIntegrator {
public:
    virtual void reset(float init_value) = 0;
    
    // Adds a new data point to the integrator
    virtual void feedData(float update_diff, unsigned long update_time) = 0;
    
    // Evaluates the integrator at eval_time.
    // If feedData() has not yet been called, this must return the initial value.
    virtual float evalResult(unsigned long eval_time) = 0;
    
    // Returns the result calculated from the last time the integrator was fed data.
    // This can be used to reduce the number of calculations.
    virtual float getLastResult() = 0;
    virtual unsigned long getLastUpdateTime() = 0;
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
    virtual float evalResult(unsigned long eval_time);
    virtual float getLastResult();
    virtual unsigned long getLastUpdateTime();
};

/*
    Based on the Adams-Bashforth multistep predictor-corrector method
    proposed for INS applications by Jose J. Rosales and Ismael Colomina http://www.isprs.org/proceedings/2005/semana_geomatica05/front/abstracts/dimarts8/N06.pdf
*/
#define ABI_BUF_SIZE 4
class AdamsBashforthIntegrator: public NumericIntegrator {
private:
    int update_count;
    double last_value; //unit
    unsigned long update_time[ABI_BUF_SIZE]; //ms
    float update_diff[ABI_BUF_SIZE]; //munit/s
    double beta[ABI_BUF_SIZE];
    
    template<typename T> void pushArray(T array[], T new_val);
    void calcBetas(unsigned long eval_time);
    float adamsBashforthStep(unsigned long tstep);
    float eulerStep(unsigned long tstep); //backup for when we don't have enough data
public:
    AdamsBashforthIntegrator();

    virtual void reset(float init_value);
    virtual void feedData(float update_diff, unsigned long update_time);
    virtual float evalResult(unsigned long eval_time);
    virtual float getLastResult();
    virtual unsigned long getLastUpdateTime();
};

#endif