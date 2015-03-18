#include "utility.h"

#include <Arduino.h>

#ifdef DBG_PRINT
char __printbuf[PRINT_BUF_SIZE];
#endif

template <typename T>
void printArray(T *array, int len, char *name) {
    Serial.print(name);
    Serial.print("[]: { ");
    for(int i = 0; i < len-1; i++) {
        Serial.print(array[i]);
        Serial.print(", ");
    }
    Serial.print(array[len-1]);
    Serial.println(" };");
}

float normalizeAngle(float angle, float max_angle) {
    float min_angle = max_angle - 360;
    while (angle <= min_angle) { angle += 360; }
    while (angle >  max_angle) { angle -= 360; }
    return angle;
}

float getShortestArc(float angle1, float angle2) {
    float arc = normalizeAngle(angle1 - angle2, 360);
    return (arc > 180.0)? 360.0 - arc : arc;
}
