#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <TimerOne.h>

#define numNotes 8

int freqMap[numNotes] = {262, 294, 330, 349, 392, 440, 494, 523};

Adafruit_MMA8451 mma = Adafruit_MMA8451();

int tonePin = 6;
int ledPin = 8;
bool heart_on = false;

int melodyFreq[512];

int count = 0;
int curr = 0;

// How long to hold notes
int dur = 75;

long lastDebounceTime = 0;
long debounceDelay = 500;

double accX = 0;
double accY = 0;

// RGB Pin
int redPin = 10;
int greenPin = 11;
int bluePin = 3;
int ledOn = -1;
 

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
  pinMode(ledPin, OUTPUT);

  // Color LED
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  

  Timer1.initialize(300000);    //700000, with note duration 150 is slowest 
  Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
  Timer1.attachInterrupt(playNote);  
}

void playNote() {
  if (heart_on) {
     digitalWrite(ledPin, LOW);
  } else {
     digitalWrite(ledPin, HIGH);
  }
  heart_on = !heart_on;
  if (curr >= count) {
    curr = 0;
  } 
  else if (count > 0) {
    generateTone(melodyFreq[curr], dur, tonePin);
    curr++;
  }
}


void loop() {
  if (ledOn > 0) {
    ledOn--;
  } else if (ledOn == 0) {
    setColor(0,0,0);
    ledOn--;
  }
  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);
  accX = event.acceleration.x;
  Serial.println(accX);
  if ((accX < -13) && ((millis() - lastDebounceTime) > debounceDelay)) {
    lastDebounceTime = millis();
    Serial.println("pulled");
    melodyFreq[count] = freqMap[int(-accX*100)%numNotes];
    count++;
    setColor(255,153,51);
    ledOn = 70;
  }
 
  accY = event.acceleration.y;
//   Serial.println(accY);
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

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
