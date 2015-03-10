
// Function used to test triangulation function

// int main() {
//     float m=0.01, b=100, t=50;
//     int res=1, lin_e=1, ang_e=1;
//     test_nav(m,b,t,res,lin_e,ang_e);
//     return 0;
// }
// void test_nav(float m, float b, float t, int res, int lin_e, int ang_e) {
//     srand (time(NULL));
//     // [n][0] = theta, [n][1] = r
//     float sensor_readings[2][100];
//     int i=0, x=0;
//     float y;
//     for (x=-t/2;x<=t&&x<100;x+=res) {
//         float noise = (rand() % (ang_e*20) - ang_e*10)*2*PI/(360*10.0);
//         y = m*x + b;
//         cout<<"y "<<y<<" x "<<x<<endl;
//         if (y == 0)
//             sensor_readings[0][i]= 0;
//         else
//             sensor_readings[0][i]=atan(x/y) + noise;
//         float noise2 = (rand() % (ang_e*20) - lin_e*10)/10.0;
//         sensor_readings[1][i++]=sqrt(x*x + y*y) + noise;
//     }
//     float new_m = 1.0/m, new_b = m*x + b - new_m*x;
//     int temp = 0;
//     for (;temp<3 && i<100;temp++) {
//         float noise = (rand() % (ang_e*20) - ang_e*10)*2*PI/(360*10.0);
//         y = -1*new_m*(x+temp/10) + new_b;
//         cout<<"y "<<y<<" x "<<x<<endl;
//         if (y == 0)
//             sensor_readings[0][i]= 0;
//         else
//             sensor_readings[0][i]=atan(x/y) + noise;
//         float noise2 = (rand() % (ang_e*20) - lin_e*10)/10.0;
//         sensor_readings[1][i++]=sqrt(x*x + y*y) + noise;
//     }
//     for (int j=0;j<i;j++) {
//         cout<<"["<<sensor_readings[1][j]<<"cm, "<<sensor_readings[0][j]*360/(2*PI)<<"]"<<endl;
//     }
//     triangulation(sensor_readings[1], sensor_readings[0], i);
// }

void polar2coor(float r, float a, float * x, float * y) {
    *x=r*sin(a);
    *y=r*cos(a);
}

bool addPoint2Regression(float x, float y, int total_points, float *sum_x, float *sum_y, int *points, float *SSxx, float *SSyy, float *SSxy) {
    float m, b;
    if (!(*SSxx)) {
        m=0;
    } else {
        m=(*SSxy)/(*SSxx);
    }
    b=(*sum_y - m*(*sum_x))/(*points);
    //cout<<m<<", "<<b<<"; "<<x<<", "<<y<<endl;
    //cout<<y - (m*x + b)<<", "<<sqrt((*SSxy)/(*points))<<endl;
    // if new point is below variance times fudge factor
    // the fudge factor becomes smaller the more samples 
    // are taken since it should begin to converge
    float scaling = (((-20.0+1.0/20)*(*points))/total_points) + 20;
    if (abs(y - (m*x + b))/scaling <= sqrt((*SSyy)/(*points)) || *points < total_points/8 + 10) {
        // update regression
        (*sum_x)+=x;
        (*sum_y)+=y;
        (*points)+=1;
        (*SSxy)+=(x - (*sum_x)/(*points))*(y - (*sum_y)/(*points));
        (*SSxx)+=(x - (*sum_x)/(*points))*(x - (*sum_x)/(*points));
        (*SSyy)+=(y - (*sum_y)/(*points))*(y - (*sum_y)/(*points));
        //cout<<(*sum_x)<<", "<<(*sum_y)<<", "<<(*points)<<", "<<(*SSxy)<<", "<<(*SSxx)<<endl;
        //cout<<*points<<", "<<(y - (m*x + b))<<", "<<sqrt((*SSxy)/(*points))<<" Accepted with scaling "<<scaling<<endl<<endl;
        return true;
    }
    cout<<y - (m*x + b)<<", "<<sqrt((*SSxy)/(*points))<<endl<<endl;
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
posn_vect triangulation(float ir_ranges[], float pan_angles[], int num_readings) {
    float sum_x=0, sum_y=0;
    float m_test=0, b_test=0, x_0=0, y_0=0, x_1=0, y_1=0;
    int edge_0=-1, edge_1=-1, midpoint=0;

    //find midpoint reading
    /* TODO could replace this loop with function parameter which gives index of 
     * central point. This point should be taken using ultrasound if base detection 
     * is to be built into this function.
     */
    float min_angle=abs(pan_angles[0]), temp;
    for (int i =0; i<num_readings; i++) {
        temp = abs(pan_angles[i]);
        if (temp < min_angle) {
            midpoint=i;
            min_angle = temp;            
        }
    }
    cout<<"midpoint: "<<midpoint<<endl<<endl;

    //Calulate rough regression to remove outliers and find edges
    float x_temp, y_temp, x_temp1, y_temp1, SSxy=0, SSxx=0, SSyy=0, x, y, m=0, b=0;
    bool is_edge_0=true, is_edge_1=false;
    int points_used=0;
    for (int i=0; midpoint+i<num_readings || midpoint-i>0; i++) {
        // Add central point (where angle == 0)
        if (i==0) {
            polar2coor(ir_ranges[midpoint], pan_angles[midpoint], &x_temp, &y_temp);
//                cout<<"r"<<ir_ranges[midpoint]<<" a"<<pan_angles[midpoint]<<" x"<<x_temp<<", y"<<y_temp<<endl;
            sum_x=abs(x_temp);
            sum_y=abs(y_temp);
                SSxy=0;
                SSxx=0;
                SSyy=0;
            points_used=1;
            i++;
        } 

        y_temp=0;
        x_temp=0;
        // Add point to the right of centre
        if (midpoint+i < num_readings) {
            cout<<"point: "<<midpoint+i<<endl;
            polar2coor(ir_ranges[midpoint+i], pan_angles[midpoint+i], &x_temp, &y_temp);
            if (addPoint2Regression(x_temp,y_temp,num_readings,&sum_x,&sum_y,&points_used,&SSxx,&SSyy,&SSxy)) {
                is_edge_0=false;
                edge_0=-1;
                //cout<<"["<<SSxy/SSxx<<","<<(sum_y - b*sum_x)/points_used<<"] Accepted"<<endl;
            } else {
                // Finding right edge
                if (!is_edge_0) {
                    is_edge_0=true;
                edge_0=midpoint+i;
                    cout<<"edge set at point"<<midpoint+i<<endl;
                } else {
                    cout<<"passed edge"<<endl;
                }
                // This data point to be ignored in final regression
                ir_ranges[midpoint+i]*=-1;
            }
        }

        // Add points to the left of centre
        y_temp=0;
        x_temp=0;
        if (midpoint-i > 0) {
            cout<<"point: "<<midpoint-i<<endl;
            polar2coor(ir_ranges[midpoint-i], pan_angles[midpoint-i], &x_temp, &y_temp);
            if (addPoint2Regression(x_temp,y_temp,num_readings,&sum_x,&sum_y,&points_used,&SSxx,&SSyy,&SSxy)) {
                is_edge_0=false;
                edge_0=-1;
                //cout<<"["<<SSxy/SSxx<<","<<(sum_y - b*sum_x)/points_used<<"] Accepted"<<endl;
            } else {
                // Finding left edge
                if (!is_edge_0) {
                    is_edge_0=true;
                    edge_0=midpoint-i;
                    cout<<"edge set at point"<<midpoint-i<<endl;
                } else {
                    cout<<"passed edge"<<endl;
                }
                // This data point to be ignored in final regression
                ir_ranges[midpoint+i]*=-1;
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
    float r_edge, a_edge;
    bool is_origin_edge = true;
    posn_vect retVal={0,0,0};
    if (is_edge_0) {
        r_edge=abs(ir_ranges[edge_0]);
        a_edge=pan_angles[edge_0];
        is_origin_edge=false;
    } else if (is_edge_1) {
        r_edge=abs(ir_ranges[edge_1]);
        a_edge=pan_angles[edge_1];
    } else {
        return retVal;
    }

    retVal.theta=atan(is_origin_edge?m:-m);
    retVal.x=r_edge*sin(PI/2 - a_edge - retVal.theta);
    retVal.y=r_edge*cos(PI/2 - a_edge - retVal.theta);
    cout<<retVal.x<<", "<<retVal.y<<", "<<retVal.theta<<endl;
    return retVal;    
}