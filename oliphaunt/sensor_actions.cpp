#include "sensor_actions.h"

#include <Arduino.h>
#include "servolib.h"
#include "sensorlib.h"
#include "action.h"
#include "actionqueue.h"
#include "utility.h"
#include "trackinglib.h"

//gets a distance reading at a certain angle with the distance IR.
void irScanAngle(float angle, float *result) {
    ActionArgs pan_args, scan_args;
    ARGS(pan_args, 0, floatval) = angle;
    ARGS(scan_args, 0, ptrval) = result;
    
    setNextAction(IRDistanceReading::instance(), &scan_args);
    setNextAction(PanIRServo::instance(), &pan_args);
}


void postRampCheck () {
    float reading = readUltraSound();
    if(reading <= TRACK_LENGHT_RAMP) {
        vector2 pos = getCurrentPosition();
        pos.y += reading;

        ActionArgs args;
        ARGS(args, 0, floatval) = pos.x;
        ARGS(args, 1, floatval) = pos.y;
        ARGS(args, 2, floatval) = 10;
        setNextAction(DriveToLocationAction::instance(), &args);
    }
    else {
        setNextAction(SweepForBase::instance(),NULL);

        ActionArgs turn_args;
        ARGS(turn_args, 0, floatval) = 270;
        setNextAction(TurnInPlaceToHeadingAction::instance(), &turn_args);
    }
}


/** SweepForBase **/

void SweepForBase::setup(ActionArgs *args) {
    scan_angle = -50.0;
    counter = 0;
    
    irScanAngle(scan_angle, &last_reading);
    suspendCurrentAction();
}

boolean SweepForBase::checkFinished() {
    if(scan_angle >= 50.0) { 
        //if we don't find anything, do stuff here.
        return true;
    }
    return false;
} 

void SweepForBase::doWork() {
    if(last_reading <= 140) {
        counter++;
    }
    else if(counter > 0) {
        counter--;
    }


    if(counter > 5) {
        //found it, go there
        vector2 target = getAbsoluteDisplacement(last_reading - 10, scan_angle);

        ActionArgs args;
        ARGS(args, 0, floatval) = normalizeAngle(getCurrentHeading() + scan_angle, 360);
        setNextAction(TurnInPlaceToHeadingAction::instance(), &args);


        ARGS(args, 0, floatval) = target.x;
        ARGS(args, 1, floatval) = target.y;
        ARGS(args, 2, floatval) = 10;
        setNextAction(DriveToLocationAction::instance(), &args);

        mountBase();        

        killCurrentAction();
        return;
    }

    scan_angle += 1.0;
    irScanAngle(scan_angle, &last_reading);
    suspendCurrentAction();
}

void SweepForBase::cleanup() {}

/** ReachBase **/

void ReachBase::setup(ActionArgs *args) {
    ActionArgs args;
    base_found = false;
    x_base = 0;
    y_base = 0;
    float measure;
    float * measure_ptr = &measure;

    is_lego_base = ARGSP(args, 0, boolval);
    // is_resume = ARGSP(args, 1, boolval);
    // is_straight = ARGSP(args, 2, boolval);

    // if (!is_resume) {
    //     suspendCurrentAction();
    //     setNextAction(this, NULL);
    //     // first check base isn't infront
    //     ARGS(args, 0, floatval *) = measure_ptr;
    //     setNextAction(UltraSoundReading::instance, &args);
        
    // }

    // if (!is_resume && (*measure_ptr) < TRACK_LENGHT_RAMP and !is_straight) {
    //     is_straight = true;
    //     suspendCurrentAction();
    //     setNextAction(this, NULL);
    //     //drive forward till within 50
            
    //     // Find Target

    //     // Drive to
    // }

    // // if (is_resume) {
    // //     suspendCurrentAction();
    // //     setNextAction(this, NULL);

    // //     ARGS(args, 1, floatval *) = measure_ptr;
    // //     setNextAction(UltraSoundReading::instance, &args);

    // //     // might skip
    // //     ARGS(args, 0, floatval) = 270; //86.5
    // //     setNextAction(TurnInPlaceToHeadingAction::instance(), &args);

    // //     if ((*measure_ptr) < TRACK_WIDTH ) {
    // //         //drive forward till within 50
            
    // //         // Find Target

    //         // Drive to
    //     }
    // }
}

void ReachBase::doWork() {
    
    ActionArgs args;
    float dist, angle;
    float * dist_ptr = &d_2_base;
    float * angle_ptr = &a_2_base;
    int * found_ret = base_found;

    suspendCurrentAction();
    setNextAction(this, NULL);

    if (!base_found) {
        vector2 move_to = getAbsoluteDisplacement(ADVANCE_INCR, 0);
        ARGS(args, 0, floatval) = move_to.x;
        ARGS(args, 1, floatval) = move_to.y;
        ARGS(args, 2, floatval) = BASE_ACCU;
        setNextAction(DriveToLocationAction::instance, &args);
    }

    // Find target
    ARGS(args, 0, boolval) = found_ret;
    ARGS(args, 1, floatval *) = dist_ptr;
    ARGS(args, 2, floatval *) = angle_ptr;
    setNextAction(FindTarget::instance, &args);
}

boolean ReachBase::checkFinished() { 
    return base_found;
}

void ReachBase::cleanup() {
    // Go to the base
    vector2 move_to = getAbsoluteDisplacement(d_2_base, a_2_base);
    ARGS(args, 0, floatval) = move_to.x;
    ARGS(args, 1, floatval) = move_to.y;
    ARGS(args, 2, floatval) = BASE_ACCU;
    setNextAction(DriveToLocationAction::instance, &args);

    // mount base... maybe
}

/** FindTarget **/

void FindTarget::setup(ActionArgs *args) {

    (PanIRServo::instance(), &pan_args);

}

void FindTarget::doWork() {

}

void FindTarget::checkFinished() {

}

void FindTarget::cleanup() {

}

/** PanIRServo **/

void PanIRServo::setup(ActionArgs *args) {
    float target_angle = ARGSP(args, 0, floatval);
    
    wait.set(estimatePanningTime(target_angle) + 5);
    setPanningServo(target_angle);
}

boolean PanIRServo::checkFinished() {
    return wait.expired();
}


/** IRDistanceReading **/

void IRDistanceReading::setup(ActionArgs *args) {
    return_ptr = (float*) ARGSP(args, 0, ptrval);
    
    reading_count = 0;
    enableDistanceIR();
    ir_refresh.set(DISTIR_REFRESH_TIME);
}

boolean IRDistanceReading::checkFinished() {
    return (reading_count >= NumReadings);
}

void IRDistanceReading::doWork() {
    if(!ir_refresh.expired()) return;
    
    //take a reading
    readings[reading_count++] = (float) getDistanceIRReading();
    ir_refresh.reset();
}

void IRDistanceReading::cleanup() {

    //calculate the average and write it to return_ptr
    float total = 0;
    for(int i = 0; i < NumReadings; i++) {
        total += readings[i];
    }
    *return_ptr = total/NumReadings;
}

/** IRSweepForTarget **/

void IRSweepForTarget::setup(ActionArgs *args) {
    leftlimit   = ARGSP(args, 0, floatval);
    rightlimit  = ARGSP(args, 1, floatval);
    nearlimit   = ARGSP(args, 2, floatval);
    farlimit    = ARGSP(args, 3, floatval);
    float resolution = ARGSP(args, 4, floatval);
    
    return_ptr  = (float*)      ARGSP(args, 5, ptrval);
    found_ptr   = (boolean*)    ARGSP(args, 6, ptrval);
    
    angle_resolution = calcResolution(resolution);
    
    target_left = leftlimit;
    target_right = leftlimit;
    found_left = false;
    found_right = false;
    
    current_angle = leftlimit; //start at the leftmost limit
    scanAngle(current_angle);
}

float IRSweepForTarget::calcResolution(float arc_spacing) {
    float avg_rad = (farlimit + nearlimit)/2.0;
    float num_scan = avg_rad/arc_spacing;
    return fabs(rightlimit - leftlimit)/num_scan;
}

//pans the IR sensor to an angle and scans it, writing to last_reading
void IRSweepForTarget::scanAngle(float angle) {
    ActionArgs pan_args;
    ARGS(pan_args, 0, floatval) = angle;
    
    ActionArgs ir_args;
    ARGS(ir_args, 0, ptrval) = &last_reading;
    
    suspendCurrentAction();
    
    //pan to the angle, take a measurement, then resume
    setNextAction(this, NULL);
    setNextAction(IRDistanceReading::instance(), &ir_args);
    setNextAction(PanIRServo::instance(), &pan_args);
}

boolean IRSweepForTarget::checkFinished() {
    //found both corners
    if(found_left && found_right) {
        *found_ptr = true;
        *return_ptr = (target_right - target_left)/2.0;
        return true;
    }
    
    if(current_angle > rightlimit) {
        //found one corner but not the other
        if(found_left) {
            *found_ptr = true;
            *return_ptr = target_left;
        }
        else {
            *found_ptr = false;
        }
        return true;
    }
    
    return false;
}

void IRSweepForTarget::doWork() {
    //check if the reading is in the search range
    if(farlimit > last_reading && last_reading > nearlimit) {
        if(!found_left) {
            //found the left corner
            //found_left = true;
            target_left = current_angle;
        }
    }
    else {
        if(found_left && !found_right) {
            //found the right corner
            //found_right = true;
            target_right = current_angle;
        }
    }
    
    //scan the next angle
    current_angle += angle_resolution;
    scanAngle(current_angle);
}

void IRSweepForTarget::cleanup() {}

/** UltraSoundReading **/

void UltraSoundReading::setup(ActionArgs *args) {
    return_ptr = (float*) ARGSP(args, 0, ptrval);
    
    reading_count = 0;
    usound_refresh.set(USOUND_CYCLE_TIME+5);
}

boolean UltraSoundReading::checkFinished() {
    return (reading_count >= NumReadings);
}

void UltraSoundReading::doWork() {
    if(!usound_refresh.expired()) return;
    
    //take a reading
    usound_refresh.reset();
    readings[reading_count++] = (float) readUltraSound();
}

void UltraSoundReading::cleanup() {
    //calculate the average and write it to return_ptr
    boolean has_reading = false;
    float total = 0;
    for(int i = 0; i < NumReadings; i++) {
        if(readings[i] > 0) {
            total += readings[i];
            has_reading = true;
        }
    }
    if(has_reading) {
        *return_ptr = total/NumReadings;
    } else {
        *return_ptr = -1.0;
    }
}