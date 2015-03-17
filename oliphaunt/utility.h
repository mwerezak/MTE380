#include <math.h>

#define SQR(x) x*x
#define CUBE(x) x*x*x
#define LINSCALE(x, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX) (x - IN_MIN)*(OUT_MAX - OUT_MIN)/(IN_MAX - IN_MIN) + OUT_MIN;

#define TORAD(x) x*M_PI/180
#define TODEG(x) x*180*M_1_PI

template <typename T>
static void printArray(T *array, int len, char *name) {
    Serial.print(name);
    Serial.print("[]: { ");
    for(int i = 0; i < len-1; i++) {
        Serial.print(array[i]);
        Serial.print(", ");
    }
    Serial.print(array[len-1]);
    Serial.println(" };");
}