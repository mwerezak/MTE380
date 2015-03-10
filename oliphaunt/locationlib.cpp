void polar2coor(float r, float a, float * x, float * y) {
    *x=r*sin(a);
    *y=r*cos(a);
}

bool addPoint2Regression(float x, float y, int total_points, float *sum_x, float *sum_y, int *points, float *SSxx, float *SSxy) {
    float m, b=(*sum_y - b*(*sum_x))/(*points);
    if (!(*SSxx)) {
        m=0;
    } else {
        m=(*SSxy)/ (*SSxx);
    }
    //cout<<m<<", "<<b<<endl;
    //cout<<y - (m*x + b)<<", "<<sqrt((*SSxy)/(*points))<<endl;
    // if new point is below variance times fudge factor
    if (abs(y - (m*x + b))/2 <= (*SSxy)/(*points) || *points <= total_points/4) {
        // update regression
        (*sum_x)+=x;
        (*sum_y)+=y;
        (*points)+=1;
        (*SSxy)+=(x - (*sum_x)/(*points))*(y - (*sum_y)/(*points));
        (*SSxx)+=(x - (*sum_x)/(*points))*(x - (*sum_x)/(*points));
      //  cout<<(*sum_x)<<", "<<(*sum_y)<<", "<<(*points)<<", "<<(*SSxy)<<", "<<(*SSxx)<<endl<<endl;
        return true;
    }
    cout<<y - (m*x + b)<<", "<<sqrt((*SSxy)/(*points))<<endl;
    return false;
}
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
void triangulation(float ir_ranges[], float pan_angles[], int num_readings) {
    float sum_x=0, sum_y=0;
    float m_test=0, b_test=0, x_0=0, y_0=0, x_1=0, y_1=0;
    int edge_0=-1, edge_1=-1, midpoint=0;

    //find midpoint reading
    /* TODO could replace this loop with function parameter which gives index of 
     * central point. This point should be taken using ultrasound if base detection 
     * is to be built into this function.
     */
    for (int i =0; i<num_readings; i++) {
        if (pan_angles[i] < 360/(2*PI) && pan_angles[i] > -360/(2*PI)) {
            midpoint=i;
            break;
        }
    }

    //Calulate regression
    float x_temp, y_temp, x_temp1, y_temp1, SSxy=0, SSxx=0, x, y, m=0, b=0;
    bool is_edge_0=true, is_edge_1=false;
    int points_used=0;
    for (int i=0; midpoint+i<num_readings || midpoint-i>0; i++) {
        // Add central point (where angle == 0)
        if (i==0) {
                polar2coor(ir_ranges[midpoint], pan_angles[midpoint], &x_temp, &y_temp);
                //cout<<"r"<<ir_ranges[midpoint]<<" a"<<pan_angles[midpoint]<<" x"<<x_temp<<", y"<<y_temp<<endl;
                sum_x=x_temp;
                sum_y=y_temp;
                SSxy=0;
                SSxx=0;
                points_used=1;
                i++;
        }

        y_temp=0;
        x_temp=0;
        // Add point to the right of centre
        if (midpoint+i < num_readings) {
            polar2coor(ir_ranges[midpoint+i], pan_angles[midpoint+i], &x_temp, &y_temp);
            if (addPoint2Regression(x_temp,y_temp,num_readings,&sum_x,&sum_y,&points_used,&SSxx,&SSxy)) {
                is_edge_0=false;
                edge_0=midpoint+i;
                //cout<<"["<<SSxy/SSxx<<","<<(sum_y - b*sum_x)/points_used<<"] Accepted"<<endl;
            } else {
                // Finding right edge
                if (!is_edge_0) {
                    is_edge_0=true;
                    edge_0=-1;
                }
                // This data point to be ignored in final regression
                ir_ranges[midpoint+i]=-1;
            }
        }

        // Add points to the left of centre
        y_temp=0;
        x_temp=0;
        if (midpoint-i > 0) {
            polar2coor(ir_ranges[midpoint-i], pan_angles[midpoint-i], &x_temp, &y_temp);
            if (addPoint2Regression(x_temp,y_temp,num_readings,&sum_x,&sum_y,&points_used,&SSxx,&SSxy)) {
                is_edge_0=false;
                edge_0=-1;
                //cout<<"["<<SSxy/SSxx<<","<<(sum_y - b*sum_x)/points_used<<"] Accepted"<<endl;
            } else {
                // Finding left edge
                if (!is_edge_0) {
                    is_edge_0=true;
                    edge_0=midpoint-i;
                }
                // This data point to be ignored in final regression
                ir_ranges[midpoint+i]=-1;
            }
        }
    }

    // Calculating Final Regression
    SSxy=0;
    SSxx=0;
    cout<<"sums ["<<sum_x<<", "<<sum_y<<", "<<points_used<<"]"<<endl;
    for (int i=0;i<num_readings;i++) {
        if (ir_ranges[i] >=0) {
            polar2coor(ir_ranges[i], pan_angles[i], &x, &y);
            SSxy+=(x - (sum_x)/(points_used))*(y - (sum_y)/(points_used));
            SSxx+=(x - (sum_x)/(points_used))*(x - (sum_x)/(points_used));
        }
    }
    m=SSxy/SSxx;
    b=(sum_y - m*sum_x)/points_used;
    cout<<"m-> "<<m<<" b-> "<<b<<endl;
    cout<<"edge 0 "<<edge_0<<" edge 1 "<<edge_1<<endl;
}