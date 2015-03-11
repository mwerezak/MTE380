#include "locationlib.h"
#include <math.h>

void polar2coor(float r, float a, float * x, float * y) {
    *x=r*sin(a);
    *y=r*cos(a);
}

void initRegressionData(RegressionData *data) {
    data->points = 0;
    data->sum_x = 0;
    data->sum_y = 0;
    data->SSxx = 0;
    data->SSyy = 0;
    data->SSxy = 0;
}

bool addPoint2Regression(float x, float y, int total_points, RegressionData *data) {
    float m, b;
    
    if (!(data->SSxx)) {
        m=0;
    } else {
        m=(data->SSxy)/(data->SSxx);
    }
    b=(data->sum_y - m*(data->sum_x))/(data->points);
    //cout<<m<<", "<<b<<"; "<<x<<", "<<y<<endl;
    //cout<<y - (m*x + b)<<", "<<sqrt((*SSxy)/(*points))<<endl;
    // if new point is below variance times fudge factor
    // the fudge factor becomes smaller the more samples 
    // are taken since it should begin to converge
    float scaling =  2; //(((-20.0+1.0/20)*(*points))/total_points) + 20;
    if (fabs(y - (m*x + b))/scaling <= sqrt(fabs(data->SSyy)/(data->points)) || data->points < 30) {
        // update regression
        (data->sum_x)+=x;
        (data->sum_y)+=y;
        (data->points)+=1;
        (data->SSxy)+=(x - (data->sum_x)/(data->points))*(y - (data->sum_y)/(data->points));
        (data->SSxx)+=(x - (data->sum_x)/(data->points))*(x - (data->sum_x)/(data->points));
        (data->SSyy)+=(y - (data->sum_y)/(data->points))*(y - (data->sum_y)/(data->points));
        //cout<<(*sum_x)<<", "<<(*sum_y)<<", "<<(*points)<<", "<<(*SSxy)<<", "<<(*SSxx)<<endl;
        //cout<<*points<<", "<<(y - (m*x + b))<<", "<<sqrt((*SSxy)/(*points))<<" Accepted with scaling "<<scaling<<endl<<endl;
        //fout<<","<<(y - (m*x + b))<<","<<sqrt((*SSxy)/(*points))<<","<<x<<","<<y<<","<<m<<","<<b<<endl;
        return true;
    }
    //cout<<y - (m*x + b)<<", "<<sqrt(fabs(data->SSxy)/(data->points))<<endl<<endl;
    return false;
}

float calculateMidpoint(float pan_angles[], int num_readings) {
    float min_angle=fabs(pan_angles[0]), temp, midpoint;
    for (int i =0; i<num_readings; i++) {
        temp = fabs(pan_angles[i]);
        if (temp < min_angle) {
            midpoint=i;
            min_angle = temp;            
        }
    }
    return midpoint;
}

posn_vect triangulation(float ir_ranges[], float pan_angles[], int num_readings) {
    float m_test=0, b_test=0, x_0=0, y_0=0, x_1=0, y_1=0;
    int edge_0=-1, edge_1=-1, midpoint=0;
    
    RegressionData data;
    initRegressionData(&data);

    //find midpoint reading
    /* TODO could replace this loop with function parameter which gives index of 
     * central point. This point should be taken using ultrasound if base detection 
     * is to be built into this function.
     */
    midpoint = calculateMidpoint(pan_angles, num_readings);
    //cout<<"midpoint: "<<midpoint<<endl<<endl;

    //Calulate rough regression to remove outliers and find edges
    float x_temp, y_temp, x_temp1, y_temp1, x, y, m=0, b=0;
    bool is_edge_0=true, is_edge_1=false;
    for (int i=0; midpoint+i<num_readings || midpoint-i>0; i++) {
        // Add central point (where angle == 0)
        if (i==0) {
            polar2coor(ir_ranges[midpoint], pan_angles[midpoint], &x_temp, &y_temp);
//                cout<<"r"<<ir_ranges[midpoint]<<" a"<<pan_angles[midpoint]<<" x"<<x_temp<<", y"<<y_temp<<endl;
            data.sum_x=fabs(x_temp);
            data.sum_y=fabs(y_temp);
            data.SSxy=0;
            data.SSxx=0;
            data.SSyy=0;
            data.points=1;
            i++;
        } 

        y_temp=0;
        x_temp=0;
        // Add point to the right of centre
        if (midpoint+i < num_readings) {
            //fout<<midpoint+i;
            polar2coor(ir_ranges[midpoint+i], pan_angles[midpoint+i], &x_temp, &y_temp);
            if (addPoint2Regression(x_temp,y_temp,num_readings,&data)) {
                is_edge_0=false;
                edge_0=-1;
                //cout<<"["<<data.SSxy/data.SSxx<<","<<(data.sum_y - b*data.sum_x)/data.points<<"] Accepted"<<endl;
            } else {
                // Finding right edge
                if (!is_edge_0) {
                    is_edge_0=true;
                edge_0=midpoint+i;
                    //cout<<"edge set at point"<<midpoint+i<<endl;
                } else {
                    //cout<<"passed edge"<<endl;
                }
                // This data point to be ignored in final regression
                ir_ranges[midpoint+i]*=-1;
            }
        }

        // Add points to the left of centre
        y_temp=0;
        x_temp=0;
        if (midpoint-i > 0) {
            //fout<<midpoint-i;
            polar2coor(ir_ranges[midpoint-i], pan_angles[midpoint-i], &x_temp, &y_temp);
            if (addPoint2Regression(x_temp,y_temp,num_readings,&data)) {
                is_edge_0=false;
                edge_0=-1;
                //cout<<"["<<data.SSxy/data.SSxx<<","<<(data.sum_y - b*data.sum_x)/data.points<<"] Accepted"<<endl;
            } else {
                // Finding left edge
                if (!is_edge_0) {
                    is_edge_0=true;
                    edge_0=midpoint-i;
                    //cout<<"edge set at point"<<midpoint-i<<endl;
                } else {
                    //cout<<"passed edge"<<endl;
                }
                // This data point to be ignored in final regression
                ir_ranges[midpoint+i]*=-1;
            }
        }
    }
    
    // Calculating Final Regression
    data.SSxy=0;
    data.SSxx=0;
    //cout<<"sums ["<<data.sum_x<<", "<<data.sum_y<<", "<<data.points<<"]"<<endl;
    for (int i=0;i<num_readings;i++) {
        if (ir_ranges[i] >=0) {
            polar2coor(ir_ranges[i], pan_angles[i], &x, &y);
            data.SSxy+=(x - (data.sum_x)/(data.points))*(y - (data.sum_y)/(data.points));
            data.SSxx+=(x - (data.sum_x)/(data.points))*(x - (data.sum_x)/(data.points));
        }
    }
    m=data.SSxy/data.SSxx;
    b=(data.sum_y - m*data.sum_x)/data.points;
    //cout<<"m-> "<<m<<" b-> "<<b<<endl;
    //cout<<"edge 0 "<<edge_0<<" edge 1 "<<edge_1<<endl;
    float r_edge, a_edge;
    bool is_origin_edge = true;
    posn_vect retVal={0,0,0};
    if (is_edge_0) {
        r_edge=fabs(ir_ranges[edge_0]);
        a_edge=pan_angles[edge_0];
        is_origin_edge=false;
    } else if (is_edge_1) {
        r_edge=fabs(ir_ranges[edge_1]);
        a_edge=pan_angles[edge_1];
    } else {
        return retVal;
    }

    retVal.theta=atan(is_origin_edge?m:-m);
    retVal.x=r_edge*sin(M_PI_2 - a_edge - retVal.theta);
    retVal.y=r_edge*cos(M_PI_2 - a_edge - retVal.theta);
    //cout<<retVal.x<<", "<<retVal.y<<", "<<retVal.theta<<endl;
    return retVal;    
}

// Function used to test triangulation function
#ifdef TRIANGULATION_DBG

int main() {
    fout.open("test_results.txt");
    float m=5, b=100, t=50;
    int res=1, lin_e=1, ang_e=1;
    //for (float i=m;i>0.01;i/=2) {
      //  for(int k=0;k<5;k++) {
            cin>>m>>t;
            fout<<"test w/ m"<<m<<" b "<<b<<" t "<<t<<" res "<<res<<endl;
            test_nav(m,b,t,res,lin_e,ang_e);
       // }
    //}
    fout.close();
    return 0;
}

void test_nav(float m, float b, float t, int res, int lin_e, int ang_e) {
    srand (time(NULL));
    // [n][0] = theta, [n][1] = r
    float sensor_readings[2][200];
    int i=0, x=0;
    float y;
    for (x=-t/2;x<=t&&x<200;x+=res) {
        float noise = (rand() % (ang_e*20) - ang_e*10)*2*M_PI/(360*10.0);
        y = m*x + b;
        //cout<<"y "<<y<<" x "<<x<<endl;
        if (y == 0)
            sensor_readings[0][i]= 0;
        else
            sensor_readings[0][i]=atan(x/y) + noise;
        float noise2 = (rand() % (ang_e*20) - lin_e*10)/10.0;
        sensor_readings[1][i++]=sqrt(x*x + y*y) + noise;
    }
    //cout<<"edge ";
    float new_m = 1.0/m, new_b = m*x + b - new_m*x;
    int temp = 0;
    for (;temp<10 && i<100;temp++) {
        cout<<i<<", ";
        float noise = (rand() % (ang_e*20) - ang_e*10)*2*M_PI/(360*10.0);
        y = -1*new_m*(x+temp/10) + new_b;
        //cout<<"y "<<y<<" x "<<x<<endl;
        if (y == 0)
            sensor_readings[0][i]= 0;
        else
            sensor_readings[0][i]=atan(x/y) + noise;
        float noise2 = (rand() % (ang_e*20) - lin_e*10)/10.0;
        sensor_readings[1][i++]=sqrt(x*x + y*y) + noise;
    }
    //cout<<endl;

    for (int j=0;j<i;j++) {
        //cout<<"["<<sensor_readings[1][j]<<"cm, "<<sensor_readings[0][j]*360/(2*M_PI)<<"]"<<endl;
    }
    triangulation(sensor_readings[1], sensor_readings[0], i);
}

#endif