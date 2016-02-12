#include "notes.h"

int pin1 = 1;
int pin0 = 0;
int tonePin = 11;
volatile int state = LOW;

long lastDebounceTime = 0;
long debounceDelay = 300;
int lastButton = LOW;
int buttonState;

long lastButtonPress = 0;

char melody[512];
int count = 0;
int curr = 0;

void setup() {
    pinMode(pin1, INPUT);
    pinMode(pin0, INPUT);
    pinMode(tonePin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(pin1), buttonPressed, RISING);
    attachInterrupt(digitalPinToInterrupt(pin0), buttonPressed, RISING);
}

void loop() {
  if (curr > count) {
    curr = 0;
  }
  Serial.println(melody[curr]);
  curr = curr + 1;

  delay(300);
}

void buttonPressed() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
        long pressed = millis();
        melody[count] = noteMap[(pressed - lastButtonPress)%numNotes];
        generateTone(freqMap[(pressed-lastButtonPress)%numNotes])
        lastButtonPress = pressed;
        count = count + 1;
        lastDebounceTime = millis();
  }
}
