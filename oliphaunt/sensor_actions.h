#include "action.h"


/*
    Parameters:
    0 - floatval: The target angle
    1 - ptrval: A pointer to a float to write the reading.
*/
class PanIRAndTakeReading : public SingletonAction<PanIRAndTakeReading> {
private:
    const static int NumReadings = 5;
    int reading_count;
    float readings[NumReadings];
    
    unsigned long wait_until;
    float *result_ptr;
public:
    virtual char* getName() { return "PanIRAndTakeReading"; }
    virtual void setup(ActionArgs *args);
    virtual boolean checkFinished();
    virtual void doWork();
    virtual void cleanup();
};