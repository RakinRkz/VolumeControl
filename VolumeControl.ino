#include "HID-Project.h"

// Rotary Encoder Module connections
const int PinDT = 2;   // DATA signal of encoder to pin 2 of arduino
const int PinCLK = 3;  // CLOCK signal of encoder to pin 4 of arduino
//if you are confused which one is DT, which one is CLK, they are interchangeable
// but will just result in opposite direction of reading


// Store previous Pins state
int PreviousCLK;
int PreviousDATA;

unsigned long currentMillis;
unsigned long prevMillis;

int counter = 0;  // Store current counter value
int prevcounter = 0;

static int oldClock = -1;  // Initialize to an impossible value.
bool clockVal;
bool dataVal;
float rpm = 0;

void setup() {
  // Serial.begin(9600);
  Consumer.begin();

  pinMode(PinDT, INPUT_PULLUP);
  pinMode(PinCLK, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PinDT), ISR_encoder, FALLING);
}
void loop() {
  // Serial.println(counter);
}


// Check if Rotary Encoder was moved
void ISR_encoder() {
  oldClock = -1;  // Initialize to an impossible value.
  clockVal = digitalRead(PinCLK);
  dataVal = digitalRead(PinDT);
  if (clockVal == oldClock) return;  // was a bounce. Don't count this.
  if (clockVal ^ dataVal) {
    // clockwise move
    counter++;
  } else {
    // counterclockwise move
    counter--;
  }
  oldClock = clockVal;  // store clock state for debounce check.

  if (counter > prevcounter + 50) {
    Consumer.write(MEDIA_VOLUME_UP);
    prevcounter = counter;
  } else if (counter < prevcounter - 50) {
    Consumer.write(MEDIA_VOLUME_DOWN);
    prevcounter = counter;
  }
}