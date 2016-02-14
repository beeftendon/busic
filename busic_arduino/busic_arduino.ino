#include <Tone.h>
#include <Wire.h>
#include <TimerOne.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#define numNotes 8

char noteMap[numNotes] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'Q'};
int freqMap[numNotes] = {262, 294, 330, 349, 392, 440, 494, 523};
int beat = 100;

Adafruit_MMA8451 mma = Adafruit_MMA8451();

int pin1 = 1;
int pin0 = 0;
int tonePin = 6;
volatile int state = LOW;

long lastDebounceTime = 0;
long debounceDelay = 1000;
int lastButton = LOW;
int buttonState;

long lastButtonPress = 0;

char melody[512];
int melodyFreq[512];
int holds[512];
int count = 0;
int curr = 0;

double accX = 0;
bool newNote = false;

void setup() {
  Serial.begin(115200);
  // Set up the accelerometer
  if (! mma.begin()) {
    while(1) {
      Serial.println("Couldnt start");
    }
  } 
  Serial.println("MMA8451 found!");
  mma.setRange(MMA8451_RANGE_2_G);

  // Set up tones
  pinMode(tonePin, OUTPUT);
}


void loop() {
  if (curr > count) {
    curr = 0;
  }
  if (newNote) {
    tone(tonePin, freqMap[0], 500);
    newNote = false;
  }

  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);
  accX = event.acceleration.x;
  if (accX > 2.0 || accX < -2.0) {
    newNote = true;
  }
  
  Serial.println(accX);

  delayMicroseconds(100);
}
