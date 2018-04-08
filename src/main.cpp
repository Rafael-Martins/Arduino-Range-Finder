#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Ultrasonic.h>

const int ledBlue = 3;
const int ledGreen = 5;
const int ledRed = 6;

const int ultrasonicTrigger = 8;
const int ultrasonicEcho = 9;

const int TIMEOUT = 15000;
const int MAX_RANGE_CM = TIMEOUT / 60;

const int D7 = 13;
const int D6 = 10;
const int D5 = 7;
const int D4 = 2;
const int ENABLE = 11;
const int RS = 12;

LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);

int counter = 0;
float previous = 0;

Ultrasonic ultrasonic(ultrasonicTrigger, ultrasonicEcho, TIMEOUT);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void draw(String row1, String row2) {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(row1);
  lcd.setCursor(3, 1);
  lcd.print(row2);
}

void printSeed(int seed) {
  Serial.println("Modify seed: " + String(seed));
  Serial.println("======================================");
  Serial.println(" ");
}

void printNormalize(float prev, float curr, float result, float maxDiff) {
  int difference = curr - prev;
  Serial.println("Previous: " + String(prev));
  Serial.println("Current: " + String(curr));
  Serial.println("Result: " + String(result));
  Serial.println("MaxDiff: " + String(maxDiff));
  Serial.println("Expected Difference: " + String(difference));
  Serial.println("Actual Difference: " + String(result - prev));
  Serial.println("======================================");
  Serial.println(" ");
}

float normalize(float prev, float curr) {
  float maxDiff = (float)prev / (float)8;
  float difference = curr - prev;
  float result = curr;

  if (difference > maxDiff) {
    result = prev + maxDiff;
  } else if (difference < - (maxDiff)) {
    result = prev - maxDiff;
  }

  return result;
}

void modifyLed(float seed) {
  seed = seed * 255;
  analogWrite(ledRed, seed * -1);
  analogWrite(ledBlue, seed);
}

void loop() {
  float range = ultrasonic.Ranging(CM);
  float normalizedRange;

  if (counter == 0) {
    normalizedRange = range;
  } else {
    normalizedRange = normalize(previous, range);
  }

  counter++;

  float rangePercentage = (float)normalizedRange / (float)MAX_RANGE_CM;
  rangePercentage = min(rangePercentage, 1);
  rangePercentage = max(rangePercentage, 0);

  previous = normalizedRange;
  modifyLed(rangePercentage);

  draw("Distance:", String((int)normalizedRange) + "cm");
  delay(10);
}


void monitor(int monitorPin = 5) {
  Serial.println("Monitor: " + String(analogRead(monitorPin)));
}
