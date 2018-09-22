#include <Arduino.h>

#define latchPin 12
#define dataPin 11
#define clockPin 8
#define cageArrivalIndicatorPin 7
#define topFloorDetectionPin 6
#define bottomFloorDetectionPin 5
#define buttonDown 4
#define buttonUp 3
#define ledDown 2
#define ledUp 1


const int datArray[9] = {3, 159, 37, 13, 153, 73, 65, 31, 1}; // array without the decimal

void setup () {
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(ledUp, OUTPUT);
  pinMode(ledDown, OUTPUT);
  pinMode(cageArrivalIndicatorPin, OUTPUT);

  pinMode(buttonDown, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(topFloorDetectionPin, INPUT);
  pinMode(bottomFloorDetectionPin, INPUT);

  digitalWrite(cageArrivalIndicatorPin, LOW);

  setFloorIndicatorDisplay(159); //writes the floor number (1) to the display

  Serial.begin(9600);
}

void loop() {
  //check status of ir sensors and buttons.
  if(isCageAtFloor()) {
    digitalWrite(cageArrivalIndicatorPin, HIGH);

    //turn off button lights ones the cage is at the floor.
    digitalWrite(ledDown, LOW);
    digitalWrite(ledUp, LOW);
  } else {
    digitalWrite(cageArrivalIndicatorPin, LOW);
  }

  if(digitalRead(buttonDown) == HIGH) {
    digitalWrite(ledDown, HIGH);
  }

  if(digitalRead(buttonUp) == HIGH) {
    digitalWrite(ledUp, HIGH);
  }

  // for (int i = 0; i <= sizeof(datArray)/sizeof(datArray[0]); i++) {
  //   setFloorIndicatorDisplay(datArray[i]);
  //   delay(1000);
  // }
}

/*
  Determines if the cage arrival indicator LED should light up. It does this by
  reading the analog input of both the ir sensors which are mounted to the floor.
*/
int isCageAtFloor() {

  int topDetectionOutput = 1 - digitalRead(topFloorDetectionPin);
  int bottomDetectionOutput = 1 - digitalRead(bottomFloorDetectionPin);
  Serial.println(topDetectionOutput);
  if (topDetectionOutput == 1 && bottomDetectionOutput == 1) {
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
