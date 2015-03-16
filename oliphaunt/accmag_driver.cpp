

#include "accmag_driver.h"

static LSM303 accmag;
static acc_raw_data ACC_OFFSET; //raw offsets
static unsigned long acc_last_update;
//static unsigned long mag_last_update;

void _printDebugAcc();

void initAccMag() {
    if(!accmag.init()) {
        Serial.println("Failed to detect accelerometer/magnetometer!");
        while (1); //trap
    }
    accmag.enableDefault();
    accmag.writeReg(LSM303::CTRL2, 0x00); //2G full scale
    accmag.writeReg(LSM303::CTRL1, 0x57); //50 Hz, BDU default, all axis enable
    
    delay(50);
    
    calibrateAccm();
    acc_last_update = millis();
}

#define ACC_CALBR_NUM_SAMPLES 128
void calibrateAccm() {
    //figure out the zero position
    long totalx = 0, totaly = 0, totalz = 0;
    for(int i=0; i < ACC_CALBR_NUM_SAMPLES; i++) {
        accmag.readAcc();
        acc_raw_data data = getAccRaw();
        totalx += data.x;
        totaly += data.y;
        totalz += data.z;
        
        delay(ACC_READ_DELAY);
    }
    ACC_OFFSET.x = totalx/ACC_CALBR_NUM_SAMPLES;
    ACC_OFFSET.y = totaly/ACC_CALBR_NUM_SAMPLES;
    ACC_OFFSET.z = totalz/ACC_CALBR_NUM_SAMPLES;
    
    #ifdef DBG_ACC
    char sbuf[80];
    snprintf(sbuf, 80, "Accm Offsets: {x: %+6d, y: %+6d, z: %+6d}", ACC_OFFSET.x, ACC_OFFSET.y, ACC_OFFSET.z);
    Serial.println(sbuf);
    delay(1500);
    #endif
}

acc_raw_data getAccRaw() {
    acc_raw_data result = accmag.a;
    result.x *= ACC_SIGN_X;
    result.y *= ACC_SIGN_Y;
    result.z *= ACC_SIGN_Z;
    return result;
}

acc_data getAccReading() {
    acc_raw_data data = getAccRaw();
    
    acc_data result;
    result.update_time = acc_last_update;
    result.x = float(data.x - ACC_OFFSET.x)*ACC_GAIN_X;
    result.y = float(data.y - ACC_OFFSET.y)*ACC_GAIN_X;
    result.z = float(data.z - ACC_OFFSET.z)*ACC_GAIN_X;
    return result;
}

boolean updateAcc() {
    if(millis() - acc_last_update >= ACC_READ_DELAY) {
        accmag.readAcc();
        acc_last_update = millis();
        
        _printDebugAcc();
        return true;
    }
    return false;
}

void _printDebugAcc() {
    #ifdef DBG_ACC
    acc_data data = getAccReading();
    
    String outstring = "                                                                ";
    outstring.setCharAt(11+((int)data.x/ACC_GAIN_X)/3277, ':');
    outstring.setCharAt(32+((int)data.y/ACC_GAIN_Y)/3277, ':');
    outstring.setCharAt(53+((int)data.z/ACC_GAIN_Z)/3277, ':');
    Serial.println(outstring);
    
    char printout[80];
    snprintf(printout, 80, "accm:     {x: %+6d, y: %+6d, z: %+6d}", int(data.x*1000), int(data.y*1000), int(data.z*1000));
    Serial.println(printout);
    #endif
}