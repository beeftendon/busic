//#include "notes.h"

#define numNotes 8

char noteMap[numNotes] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'Q'};
int freqMap[numNotes] = {262, 294, 330, 349, 392, 440, 494, 523};

void generateTone(int freq, int duration, int pin) {
  int period = 1000000/freq; // us
  int duty = period/2;
  long startTime = millis();
  long currTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(pin, 1);
    delayMicroseconds(duty); // Approximately 10% duty cycle @ 1KHz
    digitalWrite(pin, 0);
    delayMicroseconds(period - duty);
    currTime = millis();
  }
}

int pin1 = 1;
int pin0 = 0;
int tonePin = 6;
volatile int state = LOW;

long lastDebounceTime = 0;
long debounceDelay = 300;
int lastButton = LOW;
int buttonState;

long lastButtonPress = 0;

char melody[512];
int melodyFreq[512];
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
  //Serial.println(melody[curr]);
  
  int noteCount = 0;
  while (noteCount < count) {
    generateTone(melodyFreq[noteCount], 250, tonePin);
    Serial.println(melodyFreq[noteCount]);
    noteCount++;
  }
  
  curr = curr + 1;

  delay(300);
}

void buttonPressed() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
        long pressed = millis();
        melody[count] = noteMap[(pressed - lastButtonPress)%numNotes];
        melodyFreq[count] = freqMap[(pressed - lastButtonPress)%numNotes];
        //generateTone(freqMap[(pressed-lastButtonPress)%numNotes], 1000, tonePin);
        lastButtonPress = pressed;
        count = count + 1;
        lastDebounceTime = millis();
  }
}
