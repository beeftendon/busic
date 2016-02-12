#define numNotes 8

char noteMap[numNotes] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'Q'};
int freqMap[numNotes] = {262, 294, 330, 349, 392, 440, 494, 523};

void generateTone(int freq, int pin) {
  int period = 1000000/freq; // us
  int duty = period/2;
  digitalWrite(pin, 1);
  delayMicroseconds(duty); // Approximately 10% duty cycle @ 1KHz
  digitalWrite(pin, 0);
  delayMicroseconds(period - duty);
}
