#include "FastIMU.h"

MPU6500 IMU;
calData calib = { 0 };
AccelData accelData;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    IMU.init(calib, 0x68);
}

void loop() {
    IMU.update();
    IMU.getAccel(&accelData);
    float roll = atan2(accelData.accelY, sqrt(accelData.accelX * accelData.accelX + accelData.accelZ * accelData.accelZ)) * 180 / PI;
    float pitch = atan2(accelData.accelX, sqrt(accelData.accelY * accelData.accelY + accelData.accelZ * accelData.accelZ)) * 180 / PI;
    Serial.println("Roll: " + String(roll) + ", Pitch: " + String(pitch));
}
