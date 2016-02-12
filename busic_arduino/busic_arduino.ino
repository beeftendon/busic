int pin1 = 1;
int pin0 = 0;
volatile int state = LOW;

long lastDebounceTime = 0;
long debounceDelay = 300;
int lastButton = LOW;
int buttonState;

long lastButtonPress = 0;

char melody[512];
int count = 0;
int curr = 0;

char mapping[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

void setup() {
    pinMode(pin1, INPUT);
    pinMode(pin0, INPUT);
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
        melody[count] = mapping[(pressed - lastButtonPress)%7];
        lastButtonPress = pressed;
        count = count + 1;
        lastDebounceTime = millis();
  }
}
