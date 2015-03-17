
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