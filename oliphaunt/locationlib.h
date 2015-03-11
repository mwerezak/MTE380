struct posn_vect {
    //The units are cm
    float x;
    float y;
    //The units are rads
    float theta;
        //Might add tilt...
};

void polar2coor(float, float, float *, float *);
bool addPoint2Regression(float, float, int, float *, float *, int *, float *, float *, float *);
/*
 * This function will find the position of the robot relative to the edge of the 
 * course (ramp side). The lego base will be in the positive y direction and the 
 * opposing wall edge will be the negative x direction. The angle will be taken
 * between the direction of the robot and the positive y axis.
 * args:
 *    ir_ranges[] -> array with the ranges recieved from the IR sensor
 *    pan_angles[] -> array with the angles from the panning motor w/r to the
 *                    robot's direction
 *    num_readings
 */
posn_vect triangulation(float ir_ranges[], float pan_angles[], int num_readings);