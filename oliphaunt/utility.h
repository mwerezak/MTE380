
#define LINSCALE(X, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX) (X - IN_MIN)*(OUT_MAX - OUT_MIN)/(IN_MAX - IN_MIN) + OUT_MIN;

template <typename T> void printArray(T *array, int len, char *name);

float normalizeAngle(float angle, float max_angle);
float headingToBearing(float heading);
float getShortestArc(float angle1, float angle2);