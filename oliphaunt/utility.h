#include <math.h>

#define SQR(x) square(x)
#define CUBE(x) x*square(x)
#define LINSCALE(x, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX) (x - IN_MIN)*(OUT_MAX - OUT_MIN)/(IN_MAX - IN_MIN) + OUT_MIN;

#define TORAD(x) x*M_PI/180
#define TODEG(x) x*180*M_1_PI

template <typename T> void printArray(T *array, int len, char *name);

float normalizeAngle(float angle, float max_angle);
float getShortestArc(float angle1, float angle2);
