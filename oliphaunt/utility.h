#define DBG_PRINT

#ifndef _UTILITY_H
#define _UTILITY_H

#include <Arduino.h>
#include <math.h>
#include "action.h"

#define SQR(x) square(x)
#define CUBE(x) x*square(x)
#define LINSCALE(x, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX) (constrain(x, IN_MIN, IN_MAX) - IN_MIN)*(OUT_MAX - OUT_MIN)/(IN_MAX - IN_MIN) + OUT_MIN

#define TORAD(x) x*M_PI/180
#define TODEG(x) x*180*M_1_PI

#ifdef DBG_PRINT
#define PRINT_BUF_SIZE 256
extern char __printbuf[PRINT_BUF_SIZE];
#define PRINTF(format, ...) snprintf(__printbuf, PRINT_BUF_SIZE, format, __VA_ARGS__); Serial.println(__printbuf)
#else
#define PRINTF(format, ...) (void)0
#endif

template <typename T> void printArray(T *array, int len, char *name);

float normalizeAngle(float angle, float max_angle);
float getShortestArc(float angle1, float angle2);

class DelayTimer {
private:
    unsigned long last_set_time, delay_time;
public:
    DelayTimer() {};
    DelayTimer(unsigned long delay_time) { set(delay_time); }
    void set(unsigned long delay_time);
    void reset();
    boolean expired();
};

/*
    Parameters:
    0 - ulongval: The time in ms to wait
*/
class WaitAction : public SingletonAction<WaitAction> {
private:
    DelayTimer wait;
public:
    virtual char* getName() { return "WaitAction"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
};

#endif
