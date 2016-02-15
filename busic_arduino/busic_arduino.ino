#include <Tone.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <TimerOne.h>

#define numNotes 8

int freqMap[numNotes] = {262, 294, 330, 349, 392, 440, 494, 523};

Adafruit_MMA8451 mma = Adafruit_MMA8451();

int tonePin = 6;

int melodyFreq[512];

int count = 0;
int curr = 0;

// How long to hold notes
int dur = 75;

long lastDebounceTime = 0;
long debounceDelay = 500;

double accX = 0;
double accY = 0;

void generateTone(int freq, int duration, int pin) {
  int period = 1000000/freq; // us
  int duty = period/2;

  int cnt = 0; // millis and micros don't work so well in interrupts...
  while (cnt < duration) {
    digitalWrite(pin, 1);
    delayMicroseconds(duty); // Approximately 10% duty cycle @ 1KHz
    digitalWrite(pin, 0);
    delayMicroseconds(period - duty);
    cnt ++;
    }
}

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

  Timer1.initialize(300000);    //700000, with note duration 150 is slowest 
  Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
  Timer1.attachInterrupt(playNote);  
}

void playNote() {
  if (curr >= count) {
    curr = 0;
  } 
  else if (count > 0) {
    generateTone(melodyFreq[curr], dur, tonePin);
    curr++;
  }
}


void loop() {
    
  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);
  accX = event.acceleration.x;

  if ((accX < -3.0) && ((millis() - lastDebounceTime) > debounceDelay)) {
    lastDebounceTime = millis();
    melodyFreq[count] = freqMap[int(-accX*100)%numNotes];
    count++;
  }
  
  accY = event.acceleration.y;
  
  // The slowest timer interrupt period is 700000, with a note duration of 150.
  if (round(7 - abs(accY)) > 3) {
    Timer1.initialize((100000 * round(7 - abs(accY))));
    dur = int(150 * (7 - abs(accY))/7);    
  } else { // Fastest possible time
    Timer1.initialize(300000);
    dur = 75;    
  }
  
  delayMicroseconds(1000);
}
