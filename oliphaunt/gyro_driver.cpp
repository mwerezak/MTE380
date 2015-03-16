#include "gyro_driver.h"
#include <Arduino.h>

static L3G gyro;
static gyro_raw_data GYRO_OFFSET; //raw offsets
static unsigned long lastUpdateTime;

void _printDebug();

void initGyro() {
    if(!gyro.init()) {
        Serial.println("Failed to detect gyro!");
        while (1); //trap
    }
    
    gyro.enableDefault();
    gyro.writeReg(L3G::CTRL_REG4, 0x00); // 245 dps full scale
    gyro.writeReg(L3G::CTRL_REG1, 0x4F); // normal power mode, all axes enabled, 200 Hz ODR, 12.5 Hz BW
    
    delay(50);
    
    calibrateGyro();
    lastUpdateTime = millis();
}

//Updates gyro offset
#define GYRO_CALBR_NUM_SAMPLES 128
void calibrateGyro() {
    //figure out the zero position
    long totalx = 0, totaly = 0, totalz = 0;
    for(int i=0; i < GYRO_CALBR_NUM_SAMPLES; i++) {
        gyro.read();
        gyro_raw_data data = getGyroRaw();
        totalx += data.x;
        totaly += data.y;
        totalz += data.z;
        
        delay(GYRO_READ_DELAY);
    }
    GYRO_OFFSET.x = totalx/GYRO_CALBR_NUM_SAMPLES;
    GYRO_OFFSET.y = totaly/GYRO_CALBR_NUM_SAMPLES;
    GYRO_OFFSET.z = totalz/GYRO_CALBR_NUM_SAMPLES;
    
    #ifdef DBG_GYRO
    char sbuf[80];
    snprintf(sbuf, 80, "Gyro Offsets: {x: %+6d, y: %+6d, z: %+6d}", GYRO_OFFSET.x, GYRO_OFFSET.y, GYRO_OFFSET.z);
    Serial.println(sbuf);
    delay(1500);
    #endif
}

boolean updateGyro() {
    if(millis() - lastUpdateTime >= GYRO_READ_DELAY) {
        gyro.read();
        lastUpdateTime = millis();
        
        _printDebug();
        return true;
    }
    return false;
}

gyro_raw_data getGyroRaw() {
    gyro_raw_data result = gyro.g;
    result.x *= GYRO_SIGN_X;
    result.y *= GYRO_SIGN_Y;
    result.z *= GYRO_SIGN_Z;
    return result;
}

gyro_data getGyroReading() {
    gyro_raw_data data = getGyroRaw();
    
    gyro_data result;
    result.update_time = lastUpdateTime;
    result.x = float(data.x - GYRO_OFFSET.x)*GYRO_GAIN_X;
    result.y = float(data.y - GYRO_OFFSET.y)*GYRO_GAIN_Y;
    result.z = float(data.z - GYRO_OFFSET.z)*GYRO_GAIN_Z;
    return result;
}

#ifdef DBG_GYRO
#include <String.h>
#endif

void _printDebug() {
    #ifdef DBG_GYRO
    gyro_data data = getGyroReading();
    
    String outstring = "                                                                ";
    outstring.setCharAt(11+((int)data.x/GYRO_GAIN_X)/3277, ':');
    outstring.setCharAt(32+((int)data.y/GYRO_GAIN_Y)/3277, ':');
    outstring.setCharAt(53+((int)data.z/GYRO_GAIN_Z)/3277, ':');
    Serial.println(outstring);
    
    char printout[80];
    snprintf(printout, 80, "gyro:     {x: %+6d, y: %+6d, z: %+6d}", int(data.x*1000), int(data.y*1000), int(data.z*1000));
    Serial.println(printout);
    #endif
}
