//#define DBG_ACC

#ifndef _ACCMAG_DRIVER_H
#define _ACCMAG_DRIVER_H

#include <LSM303.h>

#define ACC_READ_DELAY 625 //microseconds, 1600 Hz

// LSM303 accelerometer: 2G full scale
// 0.061 mG/digit; 1G = 0.000061; 1 cm/s^2 = 0.059841
#define ACC_GAIN_X 0.059841
#define ACC_GAIN_Y 0.059841
#define ACC_GAIN_Z 0.059841

#define ACC_SIGN_X 1
#define ACC_SIGN_Y -1
#define ACC_SIGN_Z 1

#define MAG_READ_DELAY 10000 //microseconds, 100 Hz

typedef LSM303::vector<int16_t> acc_raw_data;
typedef LSM303::vector<int16_t> mag_raw_data;

typedef struct AccReadingsStruct {
    unsigned long update_time;
    float x;
    float y;
    float z;
} acc_data;

void initAccMag();
void calibrateAccm();

acc_raw_data getAccRaw();
acc_data getAccReading();
boolean updateAcc();

mag_raw_data getMagRaw();
boolean updateMag();


#endif