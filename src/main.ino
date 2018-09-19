#include <Arduino.h>

const int latchPin = 12; //Pin connected to ST_CP of 74HC595
const int clockPin = 8; //Pin connected to SH_CP of 74HC595
const int dataPin = 11; //Pin connected to DS of 74HC595
const int cageArrivalIndicatorPin = 7;
const int datArray[9] = {3, 159, 37, 13, 153, 73, 65, 31, 1}; // array without the decimal

void setup () {
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(cageArrivalIndicatorPin, OUTPUT);

  digitalWrite(cageArrivalIndicatorPin, LOW);

  setFloorIndicatorDisplay(159); //writes the floor number (1) to the display

  Serial.begin(9600);
}

void loop() {
  if(isCageAtFloor()) {
    digitalWrite(cageArrivalIndicatorPin, HIGH);
  } else {
    digitalWrite(cageArrivalIndicatorPin, LOW);
  }
}

/*
  Determines if the cage arrival indicator LED should light up. It does this by
  reading the analog input of both the ir sensors which are mounted to the floor.
*/
int isCageAtFloor() {
  const int topFloorDetectionPin = 0; //Pin connected to analog 0
  const int bottomFloorDetectionPin = 1; //Pin connected to analog 1

  int topDetectionOutput = analogRead(topFloorDetectionPin);
  int bottomDetectionOutput = analogRead(bottomFloorDetectionPin);

  if (topDetectionOutput < 300 && bottomDetectionOutput < 300) {
    return 1;
  }

  return 0;
}

/*
  Pushes the given byte to the shift register and sets the latchpin high.
*/
void setFloorIndicatorDisplay(int byte) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin,clockPin,MSBFIRST, byte);
  digitalWrite(latchPin, HIGH);
}
