#define DBG_GYRO

#ifndef _GYRO_DRIVER_H
#define _GYRO_DRIVER_H

#include <L3G.h>
#include "common.h"

#define GYRO_READ_DELAY 20 //50 Hz

// L3G4200D gyro: 245 dps full scale
// 8.75 mdps/digit; 1 dps = 0.00875
#define GYRO_GAIN_X 0.00875
#define GYRO_GAIN_Y 0.00875
#define GYRO_GAIN_Z 0.00875

#define GYRO_SIGN_X 1
#define GYRO_SIGN_Y 1
#define GYRO_SIGN_Z 1

typedef L3G::vector<int16_t> raw_gyro_data; //raw gyro data type

typedef struct GyroReadingsStruct {
    unsigned long update_time;
    float x;
    float y;
    float z;
} gyro_data;

void initGyro();
void calibrateGyro();
void updateGyro();
raw_gyro_data getGyroRaw();
gyro_data getGyroReading();


#endif