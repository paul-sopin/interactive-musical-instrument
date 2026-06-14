#include "FastIMU.h"

MPU6500 IMU;
calData calib = { 0 };
AccelData accelData;
const int buzzerPin = 8;
const float BASE_FREQ = 262.0;
const int buttonPin = 7;
int scale[] = {0, 2, 4, 5, 7, 9, 11}; // C major white keys

void setup() {
    Serial.begin(115200);
    Wire.begin();
    IMU.init(calib, 0x68);
    pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
    IMU.update();
    IMU.getAccel(&accelData);
    float roll = atan2(accelData.accelY, sqrt(accelData.accelX * accelData.accelX + accelData.accelZ * accelData.accelZ)) / PI;
    float pitch = atan2(accelData.accelX, sqrt(accelData.accelY * accelData.accelY + accelData.accelZ * accelData.accelZ)) / PI;
    // Serial.println("Roll: " + String(roll) + ", Pitch: " + String(pitch));
    int note_number = int((roll + 0.5) * 7);
    if (note_number < 0) note_number = 0;
    if (note_number > 6) note_number = 6;
    int semitone = scale[note_number];
    int octave = int((pitch + 0.5) * 3);
    if (octave < 0) octave = 0;
    if (octave > 2) octave = 2;
    float frequency = BASE_FREQ * pow(2, semitone/12.0) * pow(2, octave - 1);
    Serial.println(String(note_number) + ", Semitone: " + String(semitone) + ", Octave: " + String(octave) + ", Frequency: " + String(frequency));
    if (digitalRead(buttonPin) == HIGH) {
        noTone(buzzerPin);
    }
    else{
        tone(buzzerPin, frequency);
    }
}
