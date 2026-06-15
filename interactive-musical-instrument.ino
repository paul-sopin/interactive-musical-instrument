#include "FastIMU.h"
#include <LiquidCrystal.h>

MPU6500 IMU;
calData calib = { 0 };
AccelData accelData;
const int buzzerPin = 8;
const float BASE_FREQ = 262.0;
const int buttonPin = 7;
const int bluePin = 6;
const int greenPin = 9;
const int redPin = 10;
const int RSPin = 12;
const int ENPin = 11;
const int D7Pin = 2;
const int D6Pin = 3;
const int D5Pin = 4;
const int D4Pin = 5;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int redValues[] = {255, 255, 255, 0, 0, 75, 148};
int greenValues[] = {0, 127, 255, 255, 0, 0, 0};
int blueValues[] = {0, 0, 0, 0, 255, 130, 211};
int scale[] = {0, 2, 4, 5, 7, 9, 11}; // C major white keys
String noteNames[] = {"C", "D", "E", "F", "G", "A", "B"};


void setup() {
    Serial.begin(115200);
    Wire.begin();
    IMU.init(calib, 0x68);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(bluePin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    lcd.begin(16, 2);
    lcd.clear();
}

void loop() {
    IMU.update();
    IMU.getAccel(&accelData);
    int division_factor = 16;
    float roll = atan2(accelData.accelY, sqrt(accelData.accelX * accelData.accelX + accelData.accelZ * accelData.accelZ)) / PI;
    float pitch = atan2(accelData.accelX, sqrt(accelData.accelY * accelData.accelY + accelData.accelZ * accelData.accelZ)) / PI;
    int note_number = int((roll + 0.5) * 7);
    if (note_number < 0) note_number = 0;
    if (note_number > 6) note_number = 6;
    int semitone = scale[note_number];
    int octave = int((pitch + 0.5) * 3);
    if (octave < 0) octave = 0;
    if (octave > 2) octave = 2;
    float frequency = BASE_FREQ * pow(2, semitone/12.0) * pow(2, octave - 1);
    if (digitalRead(buttonPin) == HIGH) {
        noTone(buzzerPin);
    }
    else{
        tone(buzzerPin, frequency);
        division_factor = 8;
    }
    analogWrite(redPin, redValues[note_number]/division_factor);
    analogWrite(greenPin, greenValues[note_number]/division_factor);
    analogWrite(bluePin, blueValues[note_number]/division_factor);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Note: ");
    lcd.setCursor(0, 1);
    lcd.print(noteNames[note_number]);
    lcd.setCursor(8, 0);
    lcd.print("Octave: ");
    lcd.setCursor(15, 1);
    lcd.print(octave + 1);
    delay(100);
}
